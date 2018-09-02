# -*- ruby -*-
#
# Copyright 2017-2018 Kouhei Sutou <kou@clear-code.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

clean_white_space = lambda do |entry|
  entry.gsub(/(\A\n+|\n+\z)/, '') + "\n"
end

base_dir = __dir__
lib_dir = File.join(base_dir, "lib")
$LOAD_PATH.unshift(lib_dir)
require "arrow-pycall/version"

pycall_version = File.read(File.join(base_dir, "pycall-version")).strip

Gem::Specification.new do |spec|
  spec.name = "red-arrow-pycall"
  spec.version = ArrowPyCall::VERSION
  spec.homepage = "https://github.com/red-data-tools/red-arrow-pycall"
  spec.authors = ["Kouhei Sutou"]
  spec.email = ["kou@clear-code.com"]

  readme = File.read("README.md")
  readme.force_encoding("UTF-8")
  entries = readme.split(/^\#\#\s(.*)$/)
  clean_white_space.call(entries[entries.index("Description") + 1])
  description = clean_white_space.call(entries[entries.index("Description") + 1])
  spec.summary, spec.description, = description.split(/\n\n+/, 3)
  spec.license = "Apache-2.0"
  spec.files = ["README.md", "Rakefile", "Gemfile", "#{spec.name}.gemspec"]
  spec.files += [".yardopts"]
  spec.files += ["pycall-version"]
  spec.files += Dir.glob("lib/**/*.rb")
  spec.files += Dir.glob("ext/**/*.{c,cpp}")
  spec.files += Dir.glob("doc/text/*")
  spec.extensions = ["ext/arrow-pycall/extconf.rb"]
  spec.test_files += Dir.glob("test/**/*")

  spec.add_runtime_dependency("red-arrow")
  spec.add_runtime_dependency("red-parquet")
  spec.add_runtime_dependency("pycall", pycall_version)

  spec.add_development_dependency("bundler")
  spec.add_development_dependency("rake")
  spec.add_development_dependency("test-unit")
  spec.add_development_dependency("rake-compiler")
  spec.add_development_dependency("packnga")
  spec.add_development_dependency("kramdown")
end
