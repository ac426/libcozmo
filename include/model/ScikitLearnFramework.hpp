////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019, Vinitha Ranganeni
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     1. Redistributions of source code must retain the above copyright notice
//        this list of conditions and the following disclaimer.
//     2. Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//     3. Neither the name of the copyright holder nor the names of its
//        contributors may be used to endorse or promote products derived from
//        this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_MODEL_SCIKITLEARNFRAMEWORK_HPP_
#define INCLUDE_MODEL_SCIKITLEARNFRAMEWORK_HPP_

#include <string>
#include <model/ModelFramework.hpp>

namespace libcozmo {
namespace model {

/// Class for handling model of trained in a specific framework such as
/// scikit-learn, pytorch, etc
class ScikitLearnFramework : public virtual ModelFramework {
 public:
    /// Compiles embedded python code and loads model
    ///
    /// Throws an invalid_argument exception if the path is invalid or if the
    /// model can't be loaded correctly
    explicit ScikitLearnFramework(const std::string& model_path);

    /// Frees memory allocated by python interpreter
    ~ScikitLearnFramework();

    /// Documentation inherited
    bool initialize(const std::string& model_path) override;
};

}  // namespace model
}  // namespace libcozmo

#endif  // INCLUDE_MODEL_SCIKITLEARNFRAMEWORK_HPP_
