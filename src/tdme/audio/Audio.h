#pragma once

#if defined(__APPLE__)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#elif defined(__FreeBSD__) or defined(__linux__) or defined(_WIN32) or defined(__HAIKU__)
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include <map>
#include <string>

#include <tdme/tdme.h>
#include <tdme/audio/AudioBufferManager.h>
#include <tdme/audio/fwd-tdme.h>
#include <tdme/math/fwd-tdme.h>
#include <tdme/math/Vector3.h>

using std::map;
using std::string;

using tdme::audio::AudioBufferManager;
using tdme::audio::AudioEntity;
using tdme::math::Vector3;

/** 
 * Interface to TDME audio methods
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::audio::Audio final
{
	friend class AudioBufferManager;
	friend class AudioStream;
	friend class Sound;

private:
	static constexpr int32_t ALBUFFERID_NONE { -1 };
	static constexpr int32_t ALSOURCEID_NONE { -1 };
	static Audio* instance;

	ALCdevice* device;
	ALCcontext* context;

	map<string, AudioEntity*> audioEntities;

	AudioBufferManager audioBufferManager {  };
	Vector3 listenerPosition {  };
	Vector3 listenerVelocity {  };
	Vector3 listenerOrientationAt {  };
	Vector3 listenerOrientationUp {  };

	/**
	 * Private constructor
	 */
	Audio();

public:
	/** 
	 * @return audio singleton instance
	 */
	static Audio* getInstance();

	/** 
	 * @return listener position
	 */
	Vector3& getListenerPosition();

	/** 
	 * @return listener velocity
	 */
	Vector3& getListenerVelocity();

	/** 
	 * @return listener orientation at
	 */
	Vector3& getListenerOrientationAt();

	/** 
	 * @return listener orientation up
	 */
	Vector3& getListenerOrientationUp();

	/** 
	 * Returns an audio entity identified by given id
	 * @param id
	 * @return audio entity
	 */
	AudioEntity* getEntity(const string& id);

	/** 
	 * Adds a audio entity
	 * @param audio entity
	 */
	void addEntity(AudioEntity* entity);

	/** 
	 * Removes an audio entity
	 * @param id
	 */
	void removeEntity(const string& id);

	/** 
	 * Clears all audio entities
	 */
	void reset();

	/** 
	 * Shuts the audio down
	 */
	void shutdown();

	/** 
	 * Update and transfer audio entity states to open AL
	 */
	void update();

};
