#include <parser/ArgParser.h>
#include <archiver/Archive.h>
#include <archiver/Utility.cpp>

#include <iostream>

int main(int argc, char** argv) {
  std::vector<std::string> files;

  ArgumentParser::ArgParser parser("Archiver");
  parser.AddHelp('h', "help", "Help of programm");
  parser.AddFlag('c', "create", "Creates an arсhiver.\n");
  parser.AddFlag('l', "list", "Outputs the array of files in archiver.\n");
  parser.AddFlag('x', "extract", "Extracts files from archiver.\n");
  parser.AddFlag('a', "append", "Appends file to archiver.\n");
  parser.AddFlag('d', "delete", "Deletes file from archiver.\n");
  parser.AddFlag('A', "concatenate", "Merges two archivers to another one.\n");
  parser.AddStringArgument('f', "file", "Contains the name of archiver.\n");
  parser.AddIntArgument('b', "block-size", "Contains the byte size of single byte block.\n");
  parser.AddStringArgument('e', "every-file", "Contains every file_name.\n")
                            .MultiValue(1).Positional().StoreValues(files);

  if (!parser.Parse(argc, argv)) {
    std::cerr << "ERROR:\nIncorrect arguments.\n" << "\n";
    std::cerr << parser.HelpDescription() << "\n";
    return EXIT_FAILURE;
  }
  
  if (parser.GetFlag("create")) {
    int32_t block_size = parser.GetIntValue("block-size");
    std::string archive_name = parser.GetStringValue("file");
    Archive my_archiver(archive_name, block_size);
    my_archiver.CreateArchive(files);
    return EXIT_SUCCESS;
  }
  if (parser.GetFlag("extract")) {
    std::string archive_name = parser.GetStringValue("file");
    Archive my_archiver(archive_name);
    if (files.size()) {
      my_archiver.ExtractFiles(files);
    } else {
      my_archiver.ExtractAllFiles();
    }
    return EXIT_SUCCESS;
  }
  if (parser.GetFlag("list")) {
    int32_t block_size = 1;
    std::string archive_name = parser.GetStringValue("file");
    Archive my_archiver(archive_name, block_size);
    my_archiver.PrintFileNames();
    return EXIT_SUCCESS;
  }
  if (parser.GetFlag("append")) {
    std::string archive_name = parser.GetStringValue("file");
    Archive my_archiver(archive_name);
    my_archiver.AppendFiles(files);
    return EXIT_SUCCESS;
  }
  if (parser.GetFlag("delete")) {
    std::string archive_name = parser.GetStringValue("file");
    Archive my_archiver(archive_name);
    my_archiver.DeleteFiles(files);
    return EXIT_SUCCESS;
  }
  if (parser.GetFlag("concatenate")) {
    std::string archive_name = parser.GetStringValue("file");
    int32_t block_size = parser.GetIntValue("block-size");
    Archive my_archiver(archive_name, block_size);
    my_archiver.MergeArchives(files);
    return EXIT_SUCCESS;
  }
  std::cout << parser.HelpDescription() << "\n";

  return EXIT_FAILURE;
}
