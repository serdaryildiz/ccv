#include "ccv.h"
#include "case.h"
#include "ccv_case.h"
#include "nnc/ccv_nnc.h"
#include "3rdparty/dsfmt/dSFMT.h"

TEST_CASE("convolutional network of 5x5 on 21x31 for error backward propagation")
{
	ccv_nnc_init();
	ccv_nnc_tensor_param_t a_params = {
		.type = CCV_TENSOR_CPU_MEMORY,
		.format = CCV_TENSOR_FORMAT_NHWC,
		.dim = {
			3, 21, 31,
		},
	};
	ccv_nnc_tensor_param_t b_params = {
		.type = CCV_TENSOR_CPU_MEMORY,
		.format = CCV_TENSOR_FORMAT_NHWC,
		.dim = {
			32, 21, 31,
		},
	};
	ccv_nnc_tensor_param_t h_params = a_params;
	ccv_nnc_tensor_param_t g_params = b_params;
	ccv_nnc_tensor_param_t w_params = {
		.type = CCV_TENSOR_CPU_MEMORY,
		.format = CCV_TENSOR_FORMAT_NHWC,
		.dim = {
			3, 5, 5, 32,
		},
	};
	ccv_nnc_tensor_param_t bias_params = {
		.type = CCV_TENSOR_CPU_MEMORY,
		.format = CCV_TENSOR_FORMAT_NHWC,
		.dim = {
			32,
		},
	};
	ccv_nnc_net_param_t net_params = {
		.size = {
			.dim = {
				3, 5, 5,
			},
		},
		.convolutional = {
			.count = 32,
		},
	};
	ccv_nnc_net_hint_t hint = ccv_nnc_net_hint_guess(net_params, &a_params, 1, &b_params, 1);
	ccv_nnc_tensor_t* a = ccv_nnc_tensor_new(0, a_params, 0);
	ccv_nnc_tensor_t* b = ccv_nnc_tensor_new(0, b_params, 0);
	ccv_nnc_net_t* forw_net = ccv_nnc_net_new(0, CCV_NNC_COMPUTE_CONVOLUTIONAL_FORWARD, net_params, 0);
	ccv_nnc_tensor_t* w = ccv_nnc_tensor_new(0, w_params, 0);
	ccv_nnc_tensor_t* bias = ccv_nnc_tensor_new(0, bias_params, 0);
	ccv_nnc_tensor_t* forw_inlets[] = {
		a,
		w,
		bias,
	};
	ccv_nnc_tensor_t* forw_outlets[] = {
		b,
	};
	ccv_nnc_net_exec(forw_net, hint, 0, forw_inlets, 3, forw_outlets, 1);
	ccv_nnc_net_t* back_net = ccv_nnc_net_new(0, CCV_NNC_COMPUTE_CONVOLUTIONAL_BACKWARD, net_params, 0);
	ccv_nnc_tensor_t* gw = ccv_nnc_tensor_new(0, w_params, 0);
	ccv_nnc_tensor_t* gbias = ccv_nnc_tensor_new(0, bias_params, 0);
	ccv_nnc_tensor_t* g = ccv_nnc_tensor_new(0, g_params, 0);
	ccv_nnc_tensor_t* h = ccv_nnc_tensor_new(0, h_params, 0);
	ccv_nnc_tensor_t* back_inlets[] = {
		g,
		a,
		b,
		w,
		bias,
	};
	ccv_nnc_tensor_t* back_outlets[] = {
		gw,
		gbias,
		h,
	};
	ccv_nnc_net_exec(back_net, hint, 0, back_inlets, 5, back_outlets, 3);
	ccv_nnc_tensor_free(a);
	ccv_nnc_tensor_free(b);
	ccv_nnc_tensor_free(g);
	ccv_nnc_tensor_free(h);
	ccv_nnc_tensor_free(w);
	ccv_nnc_tensor_free(bias);
	ccv_nnc_tensor_free(gw);
	ccv_nnc_tensor_free(gbias);
	ccv_nnc_net_free(forw_net);
	ccv_nnc_net_free(back_net);
}

#include "case_main.h"
