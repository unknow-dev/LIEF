/* Copyright 2017 - 2021 R. Thomas
 * Copyright 2017 - 2021 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LIEF_PE_PARSER_H_
#define LIEF_PE_PARSER_H_

#include <set>
#include <string>
#include <vector>

#include "LIEF/visibility.h"
#include "LIEF/utils.hpp"

#include "LIEF/Abstract/Parser.hpp"
#include "LIEF/PE/enums.hpp"

namespace LIEF {
class VectorStream;

namespace PE {
class Debug;
class ResourceNode;
class Binary;
struct pe_resource_directory_table;

class LIEF_API Parser : public LIEF::Parser {

  public:
  //! @brief Maximum size of the data read
  static constexpr size_t MAX_DATA_SIZE = 3_GB;

  static constexpr size_t MAX_TLS_CALLBACKS = 3000;

  // According to https://stackoverflow.com/a/265782/87207
  static constexpr size_t MAX_DLL_NAME_SIZE = 255;

  public:
  static bool is_valid_import_name(const std::string& name);
  static bool is_valid_dll_name(const std::string& name);

  public:
  static std::unique_ptr<Binary> parse(const std::string& filename);
  static std::unique_ptr<Binary> parse(const std::vector<uint8_t>& data, const std::string& name = "");

  Parser& operator=(const Parser& copy) = delete;
  Parser(const Parser& copy)            = delete;

  private:
  Parser(const std::string& file);
  Parser(const std::vector<uint8_t>& data, const std::string& name);

  ~Parser(void);
  Parser(void);

  void init(const std::string& name = "");

  template<typename PE_T>
  void parse(void);

  void parse_exports(void);
  void parse_sections(void);

  template<typename PE_T>
  bool parse_headers(void);

  void parse_configuration(void);

  template<typename PE_T>
  void parse_data_directories(void);

  template<typename PE_T>
  void parse_import_table(void);

  void parse_export_table(void);
  void parse_debug(void);
  void parse_debug_code_view(Debug& debug_info);
  void parse_debug_pogo(Debug& debug_info);

  template<typename PE_T>
  void parse_tls(void);

  template<typename PE_T>
  void parse_load_config(void);

  void parse_relocations(void);
  void parse_resources(void);
  void parse_string_table(void);
  void parse_symbols(void);
  void parse_signature(void);
  void parse_overlay(void);
  void parse_dos_stub(void);
  void parse_rich_header(void);

  ResourceNode* parse_resource_node(
      const pe_resource_directory_table *directory_table,
      uint32_t base_offset, uint32_t current_offset, uint32_t depth = 0);


  std::unique_ptr<VectorStream> stream_;
  Binary*                       binary_{nullptr};
  PE_TYPE                       type_;
  std::set<uint32_t>            resource_visited_;
};


}
}
#endif
