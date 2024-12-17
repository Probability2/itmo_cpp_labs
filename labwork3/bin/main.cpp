#include "lib/parsing.h"
#include "lib/sandpile_model.h"

int main(int argc, char* argv[]) {
  ParsingArguments arguments;
  SandpileModel sandpile;
  ParsingResultType curr_error = ParsingResultType::kNoErrors;
  ExtensionSandpile my_extension;
  arguments = Parsing(argc, argv, curr_error);
  if (curr_error != ParsingResultType::kNoErrors) {
    CheckParsingResultType(curr_error);
  }
  sandpile = GetSandpileModel(arguments.file_name, sandpile);
  sandpile.output_directory = arguments.output_directory;
  DoIterations(arguments.max_iteration, arguments.frequency, sandpile, my_extension);
  if (my_extension.is_freq_zero) {
    SaveBMPImage(sandpile, arguments.output_directory, 1);
  }
  for (int32_t i = 0; i < sandpile.height; ++i) {
    delete[] sandpile.sands[i];
  }
  delete[] sandpile.sands;

  return EXIT_SUCCESS;
}
