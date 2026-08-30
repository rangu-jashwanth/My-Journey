"""A beat-synchronized 2D stick-person dancer."""

from dataclasses import dataclass
from pathlib import Path
import math
import time

import pygame
from pygame.math import Vector2


# Change these values to fit your song and choreography.
# Yeshanagula is commonly listed at 168 BPM, but this version is tuned
# for a tighter six-pose cycle and a more energetic performance.
BASE_BPM = 192
TURBO_BPM = 240
SKIP_SECONDS = 10
SONG_START_OFFSET = 35.0
DANCER_COUNT = 6
BEATS_PER_MEASURE = 4
POSE_BEATS = 1
WINDOW_SIZE = (1280, 720)
SONG_FOLDER = Path(__file__).parent
SONG_FILE = SONG_FOLDER / "song.mp3"
if not SONG_FILE.exists():
	available_songs = sorted(SONG_FOLDER.glob("*.mp3"))
	if len(available_songs) == 1:
		SONG_FILE = available_songs[0]
FPS = 60

BACKGROUND = (11, 16, 28)
GRID = (28, 38, 58)
WHITE = (241, 246, 255)
MINT = (98, 231, 190)
PINK = (255, 111, 166)
YELLOW = (255, 210, 92)
BLUE = (100, 177, 255)


@dataclass
class Pose:
	"""Joint locations are offsets from the character's hip."""

	head: Vector2
	neck: Vector2
	left_elbow: Vector2
	left_hand: Vector2
	right_elbow: Vector2
	right_hand: Vector2
	left_knee: Vector2
	left_foot: Vector2
	right_knee: Vector2
	right_foot: Vector2
	bob: float = 0.0
	lean: float = 0.0
	facing: float = 1.0


def point(x, y):
	return Vector2(x, y)


POSES = [
	Pose(point(0, -174), point(0, -142), point(-48, -100), point(-76, -51), point(48, -100), point(76, -51), point(-31, 28), point(-43, 101), point(31, 28), point(43, 101), bob=8),
	Pose(point(0, -178), point(0, -145), point(-61, -108), point(-130, -108), point(61, -108), point(130, -108), point(-36, 28), point(-60, 101), point(36, 28), point(60, 101), bob=11, lean=-8),
	Pose(point(0, -180), point(0, -146), point(-50, -135), point(-18, -204), point(50, -135), point(18, -204), point(-32, 28), point(-48, 103), point(32, 28), point(48, 103), bob=10, lean=6),
	Pose(point(0, -176), point(-5, -143), point(-60, -98), point(-107, -48), point(58, -99), point(99, -48), point(-35, 27), point(-65, 101), point(47, -5), point(111, -27), bob=14, lean=-12),
	Pose(point(0, -171), point(-14, -138), point(-61, -102), point(-111, -74), point(46, -100), point(77, -40), point(-53, 30), point(-103, 96), point(18, 35), point(67, 96), bob=12, lean=-18),
	Pose(point(0, -177), point(0, -144), point(-76, -91), point(-128, -35), point(76, -91), point(128, -35), point(-51, 27), point(-86, 96), point(51, 27), point(86, 96), bob=16, lean=10),
	Pose(point(0, -167), point(-10, -134), point(-40, -97), point(-18, -42), point(62, -89), point(124, -128), point(-46, 23), point(-83, 101), point(38, 29), point(71, 92), bob=8, lean=12, facing=0.9),
	Pose(point(0, -184), point(8, -152), point(-79, -125), point(-150, -187), point(63, -98), point(108, -58), point(-38, 24), point(-72, 92), point(58, 32), point(102, 105), bob=15, lean=-15, facing=1.1),
	Pose(point(0, -172), point(12, -140), point(-24, -114), point(-12, -43), point(70, -122), point(136, -164), point(-42, 30), point(-80, 98), point(32, 26), point(66, 90), bob=9, lean=18, facing=0.85),
	Pose(point(0, -181), point(-9, -151), point(-68, -100), point(-115, -44), point(74, -112), point(140, -84), point(-50, 27), point(-96, 95), point(46, 29), point(88, 104), bob=18, lean=-14, facing=1.05),
	Pose(point(0, -178), point(14, -142), point(-60, -146), point(-113, -200), point(52, -90), point(114, -50), point(-56, 28), point(-105, 94), point(24, 27), point(54, 88), bob=12, lean=9, facing=0.95),
	Pose(point(0, -170), point(-16, -138), point(-74, -88), point(-130, -44), point(42, -110), point(84, -154), point(-26, 31), point(-38, 104), point(62, 33), point(116, 103), bob=11, lean=-7, facing=1.15),
]


def interpolate_pose(first, second, amount):
	amount = amount * amount * (3.0 - 2.0 * amount)
	values = {}
	joint_names = ("head", "neck", "left_elbow", "left_hand", "right_elbow",
				   "right_hand", "left_knee", "left_foot", "right_knee",
				   "right_foot")
	for name in joint_names:
		values[name] = first.__dict__[name].lerp(second.__dict__[name], amount)
	values["bob"] = first.bob + (second.bob - first.bob) * amount
	values["lean"] = first.lean + (second.lean - first.lean) * amount
	values["facing"] = first.facing + (second.facing - first.facing) * amount
	return Pose(**values)


def pose_at_beat(beat, dancer_index=0):
	pose_index = int(beat // POSE_BEATS) % len(POSES)
	progress = (beat % POSE_BEATS) / POSE_BEATS
	pose = interpolate_pose(POSES[pose_index], POSES[(pose_index + 1) % len(POSES)], progress)
	phase = dancer_index * 0.8 + 0.2
	wiggle = math.sin(beat * (1.7 + dancer_index * 0.18) + phase)
	pose.lean += wiggle * (3.1 + dancer_index * 0.7)
	pose.bob += abs(wiggle) * (2.2 + dancer_index * 0.35)
	pose.facing += math.sin(beat * (1.2 + dancer_index * 0.12) + phase * 1.3) * 0.18
	if dancer_index % 2 == 0:
		pose.left_hand.y += math.sin(beat * 3.0 + phase) * 18
		pose.right_hand.y -= math.cos(beat * 2.8 + phase) * 16
		pose.left_foot.x += math.cos(beat * 2.2 + phase) * 10
	else:
		pose.right_hand.y += math.sin(beat * 2.6 + phase) * 20
		pose.left_hand.y -= math.cos(beat * 3.1 + phase) * 14
		pose.right_foot.x -= math.sin(beat * 2.4 + phase) * 12
	return pose


def draw_background(screen, song_time, bpm, flash, finale_mode):
	screen.fill(BACKGROUND)
	width, height = screen.get_size()
	for x in range(0, width, 64):
		pygame.draw.line(screen, GRID, (x, 0), (x, height), 1)
	for y in range(0, height, 64):
		pygame.draw.line(screen, GRID, (0, y), (width, y), 1)
	pulse = 0.5 + 0.5 * math.sin(song_time * bpm * math.pi / 30.0)
	horizon = int(height * 0.74)
	stage_glow = max(0, 30 * flash)
	pygame.draw.line(screen, (55, 71, 95), (0, horizon), (width, horizon), 2)
	pygame.draw.ellipse(screen, (18 + stage_glow, 29 + stage_glow, 43 + stage_glow), (width // 2 - 170 - int(pulse * 12), horizon - 12, 340 + int(pulse * 24), 28))

	crowd = pygame.Surface((width, height), pygame.SRCALPHA)
	crowd_bottom = height - 110
	for i in range(0, width, 16):
		person_h = 18 + int(12 * math.sin(i * 0.35 + song_time * 2.0))
		person_y = crowd_bottom - person_h
		pygame.draw.circle(crowd, (12, 16, 28, 140), (i, person_y), 8)
		pygame.draw.line(crowd, (18, 24, 38, 200), (i, person_y + 10), (i, person_y + 18), 2)
	crowd.set_alpha(180 if not finale_mode else 230)
	screen.blit(crowd, (0, 0))

	light = pygame.Surface((width, height), pygame.SRCALPHA)
	light_alpha = 80 + int(35 * math.sin(song_time * bpm * math.pi / 22.0 + 1.2))
	center_x = width // 2
	center_y = height // 2 - 30
	triad = (0.5 + 0.5 * math.sin(song_time * bpm * math.pi / 16.0))
	left_color = (int(120 + 135 * triad), int(180 + 70 * triad), 255, light_alpha)
	right_color = (int(255 - 120 * triad), int(110 + 90 * triad), int(166 + 80 * triad), light_alpha)
	pygame.draw.ellipse(light, (255, 220, 160, light_alpha), (center_x - 380, center_y - 250, 760, 520))
	pygame.draw.ellipse(light, left_color, (center_x - 620, center_y - 340, 860, 700))
	pygame.draw.ellipse(light, right_color, (center_x - 240, center_y - 340, 860, 700))
	pygame.draw.rect(light, (255, 235, 180, max(0, light_alpha // 8)), (0, 0, width, height // 3))
	pygame.draw.rect(light, (255, 235, 180, max(0, light_alpha // 9)), (0, height // 3, width, height // 3))
	light.set_alpha(160 + int(25 * flash))
	screen.blit(light, (0, 0))

	for band in range(0, width + 1, 110):
		beam_t = (song_time * bpm * math.pi / 36.0 + band / 190.0)
		beam_alpha = 35 + int(20 * math.sin(beam_t))
		beam_color = (int(255 * (0.5 + 0.5 * math.sin(beam_t + 0.5))), int(180 * (0.5 + 0.5 * math.sin(beam_t + 2.1))), int(200 * (0.5 + 0.5 * math.sin(beam_t + 4.1))), beam_alpha)
		pygame.draw.rect(light, beam_color, (band, height * 0.7, 70, height * 0.3))
	light.set_alpha(180)
	screen.blit(light, (0, 0))
	if finale_mode:
		sweep_angle = song_time * 1.2
		sweep_color = (int(255 * (0.5 + 0.5 * math.sin(sweep_angle))), int(200 * (0.5 + 0.5 * math.sin(sweep_angle + 1.0))), int(255 * (0.5 + 0.5 * math.sin(sweep_angle + 2.0))), 110)
		beam_len = width * 1.2
		end_x = center_x + math.cos(sweep_angle) * beam_len
		end_y = center_y + math.sin(sweep_angle) * beam_len * 0.7
		pygame.draw.line(screen, sweep_color, (center_x, center_y), (int(end_x), int(end_y)), 20)
	if flash > 0:
		pygame.draw.rect(screen, (255, 255, 255), (0, 0, width, height), int(3 * flash))


def draw_stick_person(screen, pose, center, song_time, bpm, turbo_mode, mirror_mode, flash):
	hip = Vector2(center)
	motion_boost = 1.45 if turbo_mode else 1.0
	bob = math.sin(song_time * bpm * math.pi / 30.0) * pose.bob * motion_boost
	lean = pose.lean + math.sin(song_time * 2.1 * motion_boost) * (2.3 if turbo_mode else 1.5)
	scale_x = pose.facing * (-1 if mirror_mode else 1)
	if flash > 0:
		pygame.draw.circle(screen, (255, 255, 255), (int(hip.x), int(hip.y - 130)), 36 + int(25 * flash), 2)

	def joint(offset):
		return hip + Vector2(offset.x * scale_x + lean, offset.y + bob)

	head = joint(pose.head)
	neck = joint(pose.neck)
	left_elbow, left_hand = joint(pose.left_elbow), joint(pose.left_hand)
	right_elbow, right_hand = joint(pose.right_elbow), joint(pose.right_hand)
	left_knee, left_foot = joint(pose.left_knee), joint(pose.left_foot)
	right_knee, right_foot = joint(pose.right_knee), joint(pose.right_foot)
	line_width = 9
	skin = (255, 226, 184)
	pygame.draw.line(screen, MINT, neck, hip, line_width)
	pygame.draw.line(screen, PINK, neck, left_elbow, line_width)
	pygame.draw.line(screen, PINK, left_elbow, left_hand, line_width)
	pygame.draw.line(screen, YELLOW, neck, right_elbow, line_width)
	pygame.draw.line(screen, YELLOW, right_elbow, right_hand, line_width)
	pygame.draw.line(screen, BLUE, hip, left_knee, line_width)
	pygame.draw.line(screen, BLUE, left_knee, left_foot, line_width)
	pygame.draw.line(screen, MINT, hip, right_knee, line_width)
	pygame.draw.line(screen, MINT, right_knee, right_foot, line_width)
	for joint_point in (neck, left_elbow, left_hand, right_elbow, right_hand, hip, left_knee, right_knee):
		pygame.draw.circle(screen, WHITE, joint_point, 7)
	pygame.draw.circle(screen, skin, head, 29)
	pygame.draw.circle(screen, WHITE, head, 29, 4)
	pygame.draw.circle(screen, BACKGROUND, head + Vector2(10 * scale_x, -5), 3)
	pygame.draw.line(screen, WHITE, left_foot, left_foot + Vector2(23 * scale_x, 0), 7)
	pygame.draw.line(screen, WHITE, right_foot, right_foot + Vector2(23 * scale_x, 0), 7)


def format_time(seconds):
	return f"{int(seconds) // 60}:{int(seconds) % 60:02d}"


def draw_ui(screen, font, small_font, song_time, beat, bpm, turbo_mode, mirror_mode, flash, finale_mode):
	width, height = screen.get_size()
	screen.blit(font.render("STICK SHIFT", True, WHITE), (34, 28))
	screen.blit(small_font.render("SIX-DANCER PERFORMANCE", True, MINT), (37, 76))
	mode_text = "TURBO" if turbo_mode else "NORMAL"
	mirror_text = "MIRROR" if mirror_mode else "STANDARD"
	finale_text = "FINALE" if finale_mode else "LIVE"
	info = small_font.render(f"BPM {bpm}   |   {mode_text}   |   {mirror_text}   |   {finale_text}   |   DANCERS {DANCER_COUNT}   |   MOVE {int(beat // POSE_BEATS) % len(POSES) + 1}/{len(POSES)}", True, (177, 193, 216))
	screen.blit(info, (width - info.get_width() - 36, 42))
	clock_text = font.render(format_time(song_time), True, WHITE)
	screen.blit(clock_text, (width - clock_text.get_width() - 36, height - 74))
	screen.blit(small_font.render("SPACE pause   K +10s   T turbo   M mirror   F flash   R restart   ESC quit", True, (177, 193, 216)), (36, height - 54))
	beat_width = 180
	pygame.draw.rect(screen, (44, 57, 79), (width // 2 - beat_width // 2, 38, beat_width, 5))
	pygame.draw.rect(screen, PINK, (width // 2 - beat_width // 2, 38, int(beat_width * (beat % 1.0)), 5))
	if flash > 0:
		screen.blit(small_font.render("FLASH!", True, WHITE), (width // 2 - 30, 84))
	if finale_mode:
		screen.blit(small_font.render("SHOW ENDING", True, YELLOW), (width // 2 - 52, 104))


def dancer_centers(width):
	spacing = width / (DANCER_COUNT + 1)
	centers = []
	for index in range(DANCER_COUNT):
		base_x = spacing * (index + 1)
		base_y = 438 + (index % 2) * 6
		centers.append((int(base_x), base_y))
	return centers


def show_missing_song(screen, font, small_font):
	screen.fill(BACKGROUND)
	heading = font.render("No MP3 song found", True, WHITE)
	detail = small_font.render("Place an MP3 beside stick dance.py and run again.", True, MINT)
	screen.blit(heading, heading.get_rect(center=(WINDOW_SIZE[0] // 2, 310)))
	screen.blit(detail, detail.get_rect(center=(WINDOW_SIZE[0] // 2, 365)))
	pygame.display.flip()


def main():
	pygame.mixer.pre_init(44100, -16, 2, 512)
	pygame.init()
	screen = pygame.display.set_mode(WINDOW_SIZE)
	pygame.display.set_caption("Stick Shift - Beat-Synchronized Dance")
	font = pygame.font.Font(None, 42)
	small_font = pygame.font.Font(None, 24)
	clock = pygame.time.Clock()

	if not SONG_FILE.exists():
		show_missing_song(screen, font, small_font)
		waiting = True
		while waiting:
			for event in pygame.event.get():
				if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
					waiting = False
			clock.tick(FPS)
		pygame.quit()
		return

	try:
		pygame.mixer.music.load(str(SONG_FILE))
		pygame.mixer.music.play(start=SONG_START_OFFSET)
		song_time = SONG_START_OFFSET
		started_at = time.perf_counter() - SONG_START_OFFSET
	except pygame.error as error:
		print(f"Could not play {SONG_FILE.name}: {error}")
		pygame.quit()
		return

	paused = False
	turbo_mode = False
	mirror_mode = False
	flash = 0.0
	finale_mode = False
	started_at = time.perf_counter() - song_time
	while True:
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				pygame.mixer.music.stop()
				pygame.quit()
				return
			if event.type == pygame.KEYDOWN:
				if event.key == pygame.K_ESCAPE:
					pygame.mixer.music.stop()
					pygame.quit()
					return
				if event.key == pygame.K_SPACE:
					paused = not paused
					if paused:
						pygame.mixer.music.pause()
					else:
						pygame.mixer.music.unpause()
						started_at = time.perf_counter() - song_time
				if event.key == pygame.K_k:
					song_time = min(max(song_time + SKIP_SECONDS, 0.0), 9999.0)
					if pygame.mixer.get_init():
						try:
							pygame.mixer.music.set_pos(song_time)
						except pygame.error:
							pass
					started_at = time.perf_counter() - song_time
					flash = 1.0
				if event.key == pygame.K_t:
					turbo_mode = not turbo_mode
				if event.key == pygame.K_m:
					mirror_mode = not mirror_mode
				if event.key == pygame.K_f:
					flash = 1.0
				if event.key == pygame.K_r:
					pygame.mixer.music.rewind()
					pygame.mixer.music.play(start=SONG_START_OFFSET)
					paused = False
					song_time = SONG_START_OFFSET
					flash = 0.0
					started_at = time.perf_counter() - song_time

		if not paused:
			mixer_time = pygame.mixer.music.get_pos() / 1000.0
			elapsed_time = time.perf_counter() - started_at
			song_time = max(0.0, mixer_time if mixer_time >= 0 else elapsed_time)
			if not pygame.mixer.music.get_busy() and song_time > 0.5:
				break
		flash = max(0.0, flash - 0.06)
		finale_mode = song_time > 65.0
		effective_bpm = TURBO_BPM if turbo_mode else BASE_BPM
		beat = song_time * effective_bpm / 60.0
		draw_background(screen, song_time, effective_bpm, flash, finale_mode)
		for index, center in enumerate(dancer_centers(WINDOW_SIZE[0])):
			mass_beat = beat + math.sin(song_time * 0.8 + index * 0.7) * 0.08
			mass_time = song_time + index * 0.04
			dancer_pose = pose_at_beat(mass_beat, index)
			if finale_mode:
				dancer_pose.bob *= 1.5
				dancer_pose.lean *= 1.3
				dancer_pose.facing += math.sin(song_time * 1.4 + index) * 0.12
			draw_stick_person(screen, dancer_pose, center, mass_time, effective_bpm, turbo_mode, mirror_mode, flash)
		draw_ui(screen, font, small_font, song_time, beat, effective_bpm, turbo_mode, mirror_mode, flash, finale_mode)
		pygame.display.flip()
		clock.tick(FPS)

	pygame.quit()


if __name__ == "__main__":
	main()
