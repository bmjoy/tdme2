#include <java/nio/ShortBuffer.h>

#include <java/lang/Byte.h>
#include <java/lang/Short.h>

using java::nio::ShortBuffer;

using java::lang::Byte;
using java::lang::Short;

extern void unimplemented_(const char16_t* name);

java::nio::ShortBuffer::ShortBuffer(const ::default_init_tag&)
	: super(*static_cast< ::default_init_tag* >(0))
{
	clinit();
}

ShortBuffer::ShortBuffer(int32_t capacity)
	: super(*static_cast< ::default_init_tag* >(0)) {
	ctor(capacity);
}

ShortBuffer::ShortBuffer(Buffer* buffer)
	: super(*static_cast< ::default_init_tag* >(0)) {
	ctor(buffer);
}

void ShortBuffer::ctor(int32_t capacity) {
	super::ctor(capacity);
}

void ShortBuffer::ctor(Buffer* buffer) {
	super::ctor(buffer);
}

int32_t ShortBuffer::position() {
	return Buffer::position() / (Short::SIZE / Byte::SIZE);
}

short ShortBuffer::get(int32_t position) {
	int16_t value = 0;
	value+= ((int16_t)super::get(position)) & 0xFF;
	value+= ((int16_t)super::get(position + 1) << 8) & 0xFF;
	return value;
}

Buffer* ShortBuffer::put(int16_t arg0) {
	int8_t* shortAsInt8 = ((int8_t*)&arg0);
	super::put(shortAsInt8[0]);
	super::put(shortAsInt8[1]);
}

Buffer* ShortBuffer::put(int16_tArray* arg0) {
	for (int i = 0; i < arg0->length; i++) {
		put(arg0->get(i));
	}
}

extern java::lang::Class* class_(const char16_t* c, int n);

java::lang::Class* ShortBuffer::class_()
{
    static ::java::lang::Class* c = ::class_(u"java.nio.ShortBuffer", 20);
    return c;
}

java::lang::Class* ShortBuffer::getClass0()
{
	return class_();
}

