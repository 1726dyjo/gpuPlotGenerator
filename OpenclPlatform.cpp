/*
	GPU plot generator for Burst coin.
	Author: Cryo
	Bitcoin: 138gMBhCrNkbaiTCmUhP9HLU9xwn5QKZgD
	Burst: BURST-YA29-QCEW-QXC3-BKXDL

	Based on the code of the official miner and dcct's plotgen.
*/

#include "OpenclError.h"
#include "OpenclPlatform.h"

namespace cryo {
namespace gpuPlotGenerator {

OpenclPlatform::OpenclPlatform(const cl_platform_id& p_handle)
: m_handle(p_handle) {
}

OpenclPlatform::OpenclPlatform(const OpenclPlatform& p_other)
: m_handle(p_other.m_handle) {
}

OpenclPlatform::~OpenclPlatform() {
}

OpenclPlatform& OpenclPlatform::operator=(const OpenclPlatform& p_other) {
	m_handle = p_other.m_handle;

	return *this;
}

std::string OpenclPlatform::getName() const throw (std::exception) {
	return getInfoString(CL_PLATFORM_NAME);
}

std::string OpenclPlatform::getVendor() const throw (std::exception) {
	return getInfoString(CL_PLATFORM_VENDOR);
}

std::string OpenclPlatform::getVersion() const throw (std::exception) {
	return getInfoString(CL_PLATFORM_VERSION);
}

std::string OpenclPlatform::getInfoString(const cl_platform_info& p_paramName) const throw (std::exception) {
	cl_int error;
	std::size_t size = 0;
	error = clGetPlatformInfo(m_handle, p_paramName, 0, 0, &size);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve info size");
	}

	char* buffer = new char[size];
	error = clGetPlatformInfo(m_handle, p_paramName, size, (void*)buffer, 0);
	if(error != CL_SUCCESS) {
		delete[] buffer;
		throw OpenclError(error, "Unable to retrieve info value");
	}

	std::string value(buffer);
	delete[] buffer;

	return value;
}

std::vector<std::shared_ptr<OpenclPlatform>> OpenclPlatform::list() throw (std::exception) {
	std::vector<std::shared_ptr<OpenclPlatform>> list;
	cl_int error;

	cl_uint platformsNumber = 0;
	error = clGetPlatformIDs(0, 0, &platformsNumber);
	if(error != CL_SUCCESS) {
		throw OpenclError(error, "Unable to retrieve the OpenCL platforms number");
	}

	cl_platform_id* platforms = new cl_platform_id[platformsNumber];
	error = clGetPlatformIDs(platformsNumber, platforms, 0);
	if(error != CL_SUCCESS) {
		delete[] platforms;
		throw OpenclError(error, "Unable to retrieve the OpenCL platforms");
	}

	for(cl_uint i = 0 ; i < platformsNumber ; ++i) {
		list.push_back(std::shared_ptr<OpenclPlatform>(new OpenclPlatform(platforms[i])));
	}

	delete[] platforms;

	return list;
}

}}
