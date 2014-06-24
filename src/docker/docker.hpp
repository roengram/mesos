/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DOCKER_HPP__
#define __DOCKER_HPP__

#include <list>
#include <string>

#include <process/future.hpp>
#include <process/subprocess.hpp>

#include <stout/json.hpp>
#include <stout/nothing.hpp>
#include <stout/option.hpp>

// Abstraction for working with Docker (modeled on CLI).
class Docker
{
public:
  // Validate Docker support
  static Try<Nothing> validateDocker(const Docker& docker);

  class Container
  {
  public:
    Container(const JSON::Object& json) : json(json) {}

    // Returns the ID of the container.
    std::string id() const;

    // Returns the name of the container.
    std::string name() const;

    // Returns the Pid of the container.
    // Note: If it returns 0, it means the container is not running.
    pid_t pid() const;

  private:
    JSON::Object json; // JSON returned from 'docker inspect'.
  };

  // Uses the specified path to the Docker CLI tool.
  Docker(const std::string& path) : path(path) {}

  // Performs 'docker run IMAGE'.
  process::Future<Option<int> > run(
      const std::string& image,
      const std::string& command,
      const std::string& name) const;

  // Performs 'docker kill CONTAINER'.
  process::Future<Option<int> > kill(
      const std::string& container) const;

  // Performs 'docker rm (-f) CONTAINER'.
  process::Future<Option<int> > rm(
      const std::string& container,
      const bool force = true) const;

  // Performs 'docker inspect CONTAINER'.
  process::Future<Container> inspect(
      const std::string& container) const;

  // Performs 'docker ps (-a)'.
  process::Future<std::list<Container> > ps(
      const bool all = true) const;

private:
  // Continuations.
  static process::Future<Container> _inspect(
      const process::Subprocess& s);
  static process::Future<std::list<Container> > _ps(
      const Docker& docker,
      const process::Subprocess& s);

  const std::string path;
};

#endif // __DOCKER_HPP__