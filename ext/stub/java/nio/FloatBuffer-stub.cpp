// Generated from /Library/Java/JavaVirtualMachines/1.6.0.jdk/Contents/Classes/classes.jar
#include <java/nio/FloatBuffer.h>

#include <java/lang/Byte.h>
#include <java/lang/Float.h>

using java::nio::FloatBuffer;

using java::lang::Byte;
using java::lang::Float;

extern void unimplemented_(const char16_t* name);

java::nio::FloatBuffer::FloatBuffer(const ::default_init_tag&)
	: super(*static_cast< ::default_init_tag* >(0))
{
	clinit();
}

FloatBuffer::FloatBuffer(int32_t capacity)
	: super(*static_cast< ::default_init_tag* >(0)) {
	ctor(capacity);
}

FloatBuffer::FloatBuffer(Buffer* buffer)
	: super(*static_cast< ::default_init_tag* >(0)) {
	ctor(buffer);
}

void FloatBuffer::ctor(int32_t capacity) {
	super::ctor(capacity);
}

void FloatBuffer::ctor(Buffer* buffer) {
	super::ctor(buffer);
}

int32_t FloatBuffer::position() {
	return Buffer::position() / (Float::SIZE / Byte::SIZE);
}

float FloatBuffer::get(int32_t position) {
	int floatAsInt = 0;
	floatAsInt+= (int32_t)super::get(position);
	floatAsInt+= (int32_t)super::get(position + 1) << 8;
	floatAsInt+= (int32_t)super::get(position + 2) << 16;
	floatAsInt+= (int32_t)super::get(position + 3) << 24;
	return *((float*)&floatAsInt);
}

Buffer* FloatBuffer::put(float arg0) {
	int8_t* floatAsInt8 = ((int8_t*)&arg0);
	super::put(floatAsInt8[0]);
	super::put(floatAsInt8[1]);
	super::put(floatAsInt8[2]);
	super::put(floatAsInt8[3]);
}

Buffer* FloatBuffer::put(floatArray* arg0) {
	for (int i = 0; i < arg0->length; i++) {
		put(arg0->get(i));
	}
}

Buffer* FloatBuffer::put(array<float, 2>* arg0) {
	for (int i = 0; i < arg0->size(); i++) {
		put((*arg0)[i]);
	}
}

Buffer* FloatBuffer::put(array<float, 3>* arg0) {
	for (int i = 0; i < arg0->size(); i++) {
		put((*arg0)[i]);
	}
}

Buffer* FloatBuffer::put(array<float, 4>* arg0) {
	for (int i = 0; i < arg0->size(); i++) {
		put((*arg0)[i]);
	}
}

extern java::lang::Class* class_(const char16_t* c, int n);

java::lang::Class* FloatBuffer::class_()
{
    static ::java::lang::Class* c = ::class_(u"java.nio.FloatBuffer", 20);
    return c;
}

java::lang::Class* FloatBuffer::getClass0()
{
	return class_();
}

