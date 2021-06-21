#ifndef Y637393029889_Z73839920239029392_A636373883
#define Y637393029889_Z73839920239029392_A636373883
namespace ilib {
struct exception {};

void (*dexception_handler)() = [](const ilib::exception& exc) { throw exc; };
}  // namespace ilib
#endif
