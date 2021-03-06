/*
Copyright (c) 2014, Intel Corporation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "../api_internal/nn_device_interface_0_internal.h"
#include "../../api/nn_primitives_api_0.h"

struct nn_workload_item;
struct nn_device_internal;

namespace layer {
class fully_connected_f32 : public nn_primitive_t {
  public:
    static fully_connected_f32 *create(size_t num_input,
                                       size_t num_output,
                                       const nn_argument_activation_t &activation,
                                       size_t batch_size,
                                       nn_device_t *device);

    fully_connected_f32(size_t num_input,
                        size_t num_output,
                        const nn_argument_activation_t &activation,
                        size_t batch_size,
                        nn_device_internal *device);

    virtual ~fully_connected_f32() {}

    virtual void forward(const nn::nn_workload_data_t<float> *input,
                         const nn::nn_workload_data_t<float> *weights,
                         const nn::nn_workload_data_t<float> *bias,
                         nn::nn_workload_data_t<float> *output);

    virtual nn::nn_workload_data_t<float> *create_weights(const nn::data<float, 2> &weights);

    virtual nn::nn_workload_data_t<float> *create_weights(const nn::data<float, 4> &weights);

    virtual nn::nn_workload_data_t<float> *create_bias(const nn::data<float, 1> &bias);

    virtual nn::nn_workload_data_t<float> *create_input(const nn::data<float, 2> &input);
    virtual nn::nn_workload_data_t<float> *create_input(const nn::data<float, 4> &input);

    virtual bool validate_input(const nn::nn_workload_data_t<float> &input);

    virtual nn::nn_workload_data_t<float> *create_output();

    virtual void copy_output(nn::data<float, 2> &destination, const nn::nn_workload_data_t<float> &source);

  private:
    template <NN_ACTIVATION_FUNCTION T_FUNCTION, bool T_NEED_BIAS_COPY>
    void run_fully_connected_work_item_internal_batch8(const nn::nn_workload_data_t<float> *input,
                                                       const nn::nn_workload_data_t<float> *weights,
                                                       const nn::nn_workload_data_t<float> *bias,
                                                       nn::nn_workload_data_t<float> *output);
    template <NN_ACTIVATION_FUNCTION T_FUNCTION, bool T_NEED_BIAS_COPY>
    void run_fully_connected_work_item_internal_batch48(const nn::nn_workload_data_t<float> *input,
                                                        const nn::nn_workload_data_t<float> *weights,
                                                        const nn::nn_workload_data_t<float> *bias,
                                                        nn::nn_workload_data_t<float> *output);
    template <NN_ACTIVATION_FUNCTION T_FUNCTION, bool T_NEED_BIAS_COPY>
    void run_fully_connected_work_item_internal_latency(const nn::nn_workload_data_t<float> *input,
                                                        const nn::nn_workload_data_t<float> *weights,
                                                        const nn::nn_workload_data_t<float> *bias,
                                                        nn::nn_workload_data_t<float> *output);
    template <NN_ACTIVATION_FUNCTION T_FUNCTION, bool T_NEED_BIAS_COPY>
    void choose_fully_connected_work_item_batching_mode(const nn::nn_workload_data_t<float> *input,
                                                        const nn::nn_workload_data_t<float> *weights,
                                                        const nn::nn_workload_data_t<float> *bias,
                                                        nn::nn_workload_data_t<float> *output);
    template <bool T_NEED_BIAS_COPY>
    void choose_fully_connected_work_item_activation(const nn::nn_workload_data_t<float> *input,
                                                     const nn::nn_workload_data_t<float> *weights,
                                                     const nn::nn_workload_data_t<float> *bias,
                                                     nn::nn_workload_data_t<float> *output);
    void run_fully_connected_work_item(const nn::nn_workload_data_t<float> *input,
                                       const nn::nn_workload_data_t<float> *weights,
                                       const nn::nn_workload_data_t<float> *bias,
                                       nn::nn_workload_data_t<float> *output);
    friend void unpack_fully_connected_callback_handle(void *void_handle);
    friend void wrapper_fully_connected_work_item(nn_workload_item *const work_item);

  protected:
    const size_t num_input, num_output, batch_size;
    const nn_argument_activation_t activation;
    nn_device_internal *device;

    static const nn_workload_data_layout_t in_out_layout;
};

void wrapper_fully_connected_work_item(nn_workload_item *const work_item);
}
