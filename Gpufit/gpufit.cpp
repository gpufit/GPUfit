#include "gpufit.h"
#include "interface.h"
#include <iostream>
#include <string>

//#include <cstdint>
//#include <limits>
#include <stdexcept>

#ifndef __int32
#define __int32  int32_t
#endif


std::string last_error ;

int gpufit
(
    size_t n_fits,
    size_t n_points,
    float * data,
    float * weights,
    ModelID model_id,
    float * initial_parameters,
    float tolerance,
    int max_n_iterations,
    int * parameters_to_fit,
    int estimator_id,
    size_t user_info_size,
    char * user_info,
    float * output_parameters,
    int * output_states,
    float * output_chi_squares,
    int * output_n_iterations,
    float * output_data
)
try
{
    __int32 n_points_32 = 0;
    if (n_points <= (unsigned int)(std::numeric_limits<__int32>::max()))
    {
        n_points_32 = __int32(n_points);
    }
    else
    {
        throw std::runtime_error("maximum number of data points per fit exceeded");
    }

    FitInterface fi(
        data,
        weights,
        n_fits,
        n_points_32,
        tolerance,
        max_n_iterations,
        static_cast<EstimatorID>(estimator_id),
        initial_parameters,
        parameters_to_fit,
        user_info,
        user_info_size,
        output_parameters,
        output_states,
        output_chi_squares,
        output_n_iterations,
        output_data);

    fi.fit(static_cast<ModelID>(model_id));

    return ReturnState::OK  ;
}
catch( std::exception & exception )
{
    last_error = exception.what() ;

    return ReturnState::ERROR  ;
}
catch( ... )
{
    last_error = "unknown error" ;

    return ReturnState::ERROR ;
}

char const * gpufit_get_last_error()
{
    return last_error.c_str() ;
}



int gpusimul
(
    size_t n_fits,
    size_t n_points,
    ModelID model_id,
    float * parameters,
    size_t user_info_size,
    char * user_info,
    float * output_data
)
try
{
    std::vector< float > temp_float = {0};
    std::vector< int > temp_int = {0};

    __int32 n_points_32 = 0;
    float * data = temp_float.data() ;
    float * weights = temp_float.data() ;
    float tolerance = 0 ;
    int max_n_iterations = 1 ;
    int * parameters_to_fit =  temp_int.data() ;
    EstimatorID estimator_id = LSE ;
    float * output_parameters = temp_float.data() ;
    int * output_states = temp_int.data();
    float * output_chi_squares = temp_float.data() ;
    int * output_n_iterations = temp_int.data() ;

    if (n_points <= (unsigned int)(std::numeric_limits<__int32>::max()))
    {
        n_points_32 = __int32(n_points);
    }
    else
    {
        throw std::runtime_error("maximum number of data points per fit exceeded");
    }

    FitInterface fi(
        data,
        weights,
        n_fits,
        n_points_32,
        tolerance,
        max_n_iterations,
        estimator_id,
        parameters,
        parameters_to_fit,
        user_info,
        user_info_size,
        output_parameters,
        output_states,
        output_chi_squares,
        output_n_iterations,
        output_data);

    fi.simulate(model_id);

    return ReturnState::OK  ;
}
catch( std::exception & exception )
{
    last_error = exception.what() ;

    return ReturnState::ERROR  ;
}
catch( ... )
{
    last_error = "unknown error" ;

    return ReturnState::ERROR ;
}




int gpufit_cuda_available()
{
	try
	{
		getDeviceCount();
		return 1;
	}
	catch (std::exception & exception)
	{
		last_error = exception.what();

		return 0;
	}
}

int gpufit_get_cuda_version(int * runtime_version, int * driver_version)
{
    try
    {
        cudaRuntimeGetVersion(runtime_version);
        cudaDriverGetVersion(driver_version);
        return ReturnState::OK;
    }
    catch (std::exception & exception)
    {
        last_error = exception.what();

        return ReturnState::ERROR;
    }
}

int gpufit_portable_interface(int argc, void *argv[])
{

    return gpufit(
        *((size_t *) argv[0]),
        *((size_t *) argv[1]),
        (float *) argv[2],
        (float *) argv[3],
        *((ModelID *) argv[4]),
        (float *) argv[5],
        *((float *) argv[6]),
        *((int *) argv[7]),
        (int *) argv[8],
        *((int *) argv[9]),
        *((size_t *) argv[10]),
        (char *) argv[11],
        (float *) argv[12],
        (int *) argv[13],
        (float *) argv[14],
        (int *) argv[15],
        (float *) argv[16]
        );
}
