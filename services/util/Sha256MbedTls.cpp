#include "services/util/Sha256MbedTls.hpp"

namespace services
{
    void Sha256MbedTls::Calculate(infra::ConstByteRange input, std::array<uint8_t, 32>& output)
    {
        mbedtls_sha256(input.begin(), input.size(), output.data(), 0);
    }
}
