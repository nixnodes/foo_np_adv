#pragma once

enum events {
	EVENT_PLAYBACK_STARTING = 0,
	EVENT_PLAYBACK_NEW_TRACK,
	EVENT_PLAYBACK_STOP,
	EVENT_PLAYBACK_SEEK,
	EVENT_PLAYBACK_PAUSE,
	EVENT_PLAYBACK_EDITED,
	EVENT_PLAYBACK_DINFO,
	EVENT_PLAYBACK_DINFO_TRACK,
	EVENT_PLAYBACK_TIME,
	EVENT_VOLCHANGE,
	EVENT_COUNT
};
