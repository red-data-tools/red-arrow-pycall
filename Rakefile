# -*- ruby -*-
#
# Copyright 2017 Kouhei Sutou <kou@clear-code.com>
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

require "rubygems"
require "bundler/gem_helper"
require "rake/extensiontask"
require "packnga"

base_dir = File.join(File.dirname(__FILE__))

helper = Bundler::GemHelper.new(base_dir)
def helper.version_tag
  version
end

helper.install
spec = helper.gemspec

Rake::ExtensionTask.new("arrow_pycall", spec) do |ext|
  ext.ext_dir = "ext/arrow-pycall"
  ext.cross_platform = ["x86-mingw32", "x64-mingw32"]
  ext.cross_compile = true
end

Packnga::DocumentTask.new(spec) do |task|
  task.original_language = "en"
  task.translate_language = "ja"
end

Packnga::ReleaseTask.new(spec) do
end

desc "Run tests"
task :test => :compile do
  ruby("test/run-test.rb")
end

task default: :test
