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

require "mkmf-gnome2"

unless required_pkg_config_package("arrow-glib")
  exit(false)
end
unless required_pkg_config_package("arrow-python")
  unless NativePackageInstaller.install(:debian => "libarrow-python-dev",
                                        :redhat => "arrow-python-devel")
    exit(false)
  end
end

[
  ["glib2", "ext/glib2"],
].each do |name, source_dir|
  spec = find_gem_spec(name)
  source_dir = File.join(spec.full_gem_path, source_dir)
  build_dir = source_dir
  add_depend_package_path(name, source_dir, build_dir)
end

checking_for(checking_message("pycall"), "%s") do
  pycall_version_path = File.join(__dir__, "..", "..", "pycall-version")
  pycall_version = File.read(pycall_version_path).strip
  pycall_spec = Gem::Specification.find_by_name("pycall", pycall_version)
  $INCFLAGS += " -I#{pycall_spec.gem_dir}/ext/pycall"
  pycall_spec.version
end

create_makefile("arrow_pycall")
