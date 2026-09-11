/*
 * Stick Shift - Beat-Synchronized 2D Stick-Man Dance Animation
 * C++17 with SFML 3.x
 * 
 * A complete port of the Python stick dance animation with enhanced
 * graphics, music synchronization, and interactive controls.
 * 
 * Controls:
 *   SPACE  = Pause/Resume
 *   K      = Skip forward 10 seconds
 *   T      = Toggle Turbo mode
 *   M      = Toggle Mirror mode
 *   F      = Flash effect
 *   R      = Restart song
 *   ESC    = Quit
 */

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

// ============================================================================
// CONFIGURATION - Modify these values to customize the animation
// ============================================================================

const float BASE_BPM = 192.0f;
const float TURBO_BPM = 240.0f;
const float SKIP_SECONDS = 10.0f;
const float SONG_START_OFFSET = 35.0f;
const int DANCER_COUNT = 6;
const int BEATS_PER_MEASURE = 4;
const float POSE_BEATS = 1.0f;
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const unsigned int FPS = 60;
const std::string SONG_FILENAME = "song.mp3";

// Color palette
const sf::Color BACKGROUND = sf::Color(11, 16, 28);
const sf::Color GRID_COLOR = sf::Color(28, 38, 58);
const sf::Color WHITE = sf::Color(241, 246, 255);
const sf::Color MINT = sf::Color(98, 231, 190);
const sf::Color PINK = sf::Color(255, 111, 166);
const sf::Color YELLOW = sf::Color(255, 210, 92);
const sf::Color BLUE = sf::Color(100, 177, 255);
const sf::Color SKIN = sf::Color(255, 226, 184);

// Dancer positioning
const float STAGE_Y = 438.0f;
const int HEAD_RADIUS = 29;
const int JOINT_RADIUS = 7;
const int LINE_WIDTH = 9;

// ============================================================================
// POSE DEFINITION
// ============================================================================

struct Vector2D {
    float x = 0.0f;
    float y = 0.0f;

    Vector2D() = default;
    Vector2D(float x, float y) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D lerp(const Vector2D& other, float t) const {
        return Vector2D(x + (other.x - x) * t, y + (other.y - y) * t);
    }

    sf::Vector2f toSF() const {
        return sf::Vector2f(x, y);
    }
};

struct Pose {
    Vector2D head, neck;
    Vector2D left_elbow, left_hand;
    Vector2D right_elbow, right_hand;
    Vector2D left_knee, left_foot;
    Vector2D right_knee, right_foot;
    float bob = 0.0f;
    float lean = 0.0f;
    float facing = 1.0f;
};

// Dance poses - these define the keyframe animation
// Each pose is a set of joint positions (offsets from hip)
const std::vector<Pose> POSES = {
    {Vector2D(0, -174), Vector2D(0, -142), Vector2D(-48, -100), Vector2D(-76, -51), Vector2D(48, -100), Vector2D(76, -51), Vector2D(-31, 28), Vector2D(-43, 101), Vector2D(31, 28), Vector2D(43, 101), 8, 0, 1},
    {Vector2D(0, -178), Vector2D(0, -145), Vector2D(-61, -108), Vector2D(-130, -108), Vector2D(61, -108), Vector2D(130, -108), Vector2D(-36, 28), Vector2D(-60, 101), Vector2D(36, 28), Vector2D(60, 101), 11, -8, 1},
    {Vector2D(0, -180), Vector2D(0, -146), Vector2D(-50, -135), Vector2D(-18, -204), Vector2D(50, -135), Vector2D(18, -204), Vector2D(-32, 28), Vector2D(-48, 103), Vector2D(32, 28), Vector2D(48, 103), 10, 6, 1},
    {Vector2D(0, -176), Vector2D(-5, -143), Vector2D(-60, -98), Vector2D(-107, -48), Vector2D(58, -99), Vector2D(99, -48), Vector2D(-35, 27), Vector2D(-65, 101), Vector2D(47, -5), Vector2D(111, -27), 14, -12, 1},
    {Vector2D(0, -171), Vector2D(-14, -138), Vector2D(-61, -102), Vector2D(-111, -74), Vector2D(46, -100), Vector2D(77, -40), Vector2D(-53, 30), Vector2D(-103, 96), Vector2D(18, 35), Vector2D(67, 96), 12, -18, 1},
    {Vector2D(0, -177), Vector2D(0, -144), Vector2D(-76, -91), Vector2D(-128, -35), Vector2D(76, -91), Vector2D(128, -35), Vector2D(-51, 27), Vector2D(-86, 96), Vector2D(51, 27), Vector2D(86, 96), 16, 10, 1},
    {Vector2D(0, -167), Vector2D(-10, -134), Vector2D(-40, -97), Vector2D(-18, -42), Vector2D(62, -89), Vector2D(124, -128), Vector2D(-46, 23), Vector2D(-83, 101), Vector2D(38, 29), Vector2D(71, 92), 8, 12, 0.9f},
    {Vector2D(0, -184), Vector2D(8, -152), Vector2D(-79, -125), Vector2D(-150, -187), Vector2D(63, -98), Vector2D(108, -58), Vector2D(-38, 24), Vector2D(-72, 92), Vector2D(58, 32), Vector2D(102, 105), 15, -15, 1.1f},
    {Vector2D(0, -172), Vector2D(12, -140), Vector2D(-24, -114), Vector2D(-12, -43), Vector2D(70, -122), Vector2D(136, -164), Vector2D(-42, 30), Vector2D(-80, 98), Vector2D(32, 26), Vector2D(66, 90), 9, 18, 0.85f},
    {Vector2D(0, -181), Vector2D(-9, -151), Vector2D(-68, -100), Vector2D(-115, -44), Vector2D(74, -112), Vector2D(140, -84), Vector2D(-50, 27), Vector2D(-96, 95), Vector2D(46, 29), Vector2D(88, 104), 18, -14, 1.05f},
    {Vector2D(0, -178), Vector2D(14, -142), Vector2D(-60, -146), Vector2D(-113, -200), Vector2D(52, -90), Vector2D(114, -50), Vector2D(-56, 28), Vector2D(-105, 94), Vector2D(24, 27), Vector2D(54, 88), 12, 9, 0.95f},
    {Vector2D(0, -170), Vector2D(-16, -138), Vector2D(-74, -88), Vector2D(-130, -44), Vector2D(42, -110), Vector2D(84, -154), Vector2D(-26, 31), Vector2D(-38, 104), Vector2D(62, 33), Vector2D(116, 103), 11, -7, 1.15f},
};

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

float smoothstep(float t) {
    // Smoothstep interpolation: 3t^2 - 2t^3
    return t * t * (3.0f - 2.0f * t);
}

Pose interpolate_pose(const Pose& first, const Pose& second, float t) {
    t = smoothstep(t);
    
    Pose result;
    result.head = first.head.lerp(second.head, t);
    result.neck = first.neck.lerp(second.neck, t);
    result.left_elbow = first.left_elbow.lerp(second.left_elbow, t);
    result.left_hand = first.left_hand.lerp(second.left_hand, t);
    result.right_elbow = first.right_elbow.lerp(second.right_elbow, t);
    result.right_hand = first.right_hand.lerp(second.right_hand, t);
    result.left_knee = first.left_knee.lerp(second.left_knee, t);
    result.left_foot = first.left_foot.lerp(second.left_foot, t);
    result.right_knee = first.right_knee.lerp(second.right_knee, t);
    result.right_foot = first.right_foot.lerp(second.right_foot, t);
    
    result.bob = first.bob + (second.bob - first.bob) * t;
    result.lean = first.lean + (second.lean - first.lean) * t;
    result.facing = first.facing + (second.facing - first.facing) * t;
    
    return result;
}

Pose pose_at_beat(float beat, int dancer_index) {
    int pose_index = static_cast<int>(beat / POSE_BEATS) % POSES.size();
    float progress = std::fmod(beat, POSE_BEATS) / POSE_BEATS;
    
    Pose pose = interpolate_pose(POSES[pose_index], POSES[(pose_index + 1) % POSES.size()], progress);
    
    // Add dynamic wiggle and movement per dancer
    float phase = dancer_index * 0.8f + 0.2f;
    float wiggle = std::sin(beat * (1.7f + dancer_index * 0.18f) + phase);
    
    pose.lean += wiggle * (3.1f + dancer_index * 0.7f);
    pose.bob += std::abs(wiggle) * (2.2f + dancer_index * 0.35f);
    pose.facing += std::sin(beat * (1.2f + dancer_index * 0.12f) + phase * 1.3f) * 0.18f;
    
    // Alternate hand and foot movements per dancer
    if (dancer_index % 2 == 0) {
        pose.left_hand.y += std::sin(beat * 3.0f + phase) * 18.0f;
        pose.right_hand.y -= std::cos(beat * 2.8f + phase) * 16.0f;
        pose.left_foot.x += std::cos(beat * 2.2f + phase) * 10.0f;
    } else {
        pose.right_hand.y += std::sin(beat * 2.6f + phase) * 20.0f;
        pose.left_hand.y -= std::cos(beat * 3.1f + phase) * 14.0f;
        pose.right_foot.x -= std::sin(beat * 2.4f + phase) * 12.0f;
    }
    
    return pose;
}

std::string format_time(float seconds) {
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    std::stringstream ss;
    ss << mins << ":" << std::setfill('0') << std::setw(2) << secs;
    return ss.str();
}

// ============================================================================
// GRAPHICS RENDERING
// ============================================================================

class StickRenderer {
public:
    StickRenderer() = default;

    void draw_line(sf::RenderWindow& window, sf::Vector2f from, sf::Vector2f to, 
                   const sf::Color& color, float thickness) {
        sf::Vector2f delta = to - from;
        float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        
        if (length < 0.1f) return;
        
        sf::RectangleShape line(sf::Vector2f(length, thickness));
        line.setPosition(from);
        line.setFillColor(color);
        
        float angle = std::atan2(delta.y, delta.x) * 180.0f / 3.14159265f;
        line.setRotation(sf::degrees(angle));
        line.setOrigin(sf::Vector2f(0, thickness / 2.0f));
        
        window.draw(line);
    }

    void draw_circle(sf::RenderWindow& window, sf::Vector2f center, float radius, 
                     const sf::Color& color, float outline_thickness = 0.0f) {
        sf::CircleShape circle(radius);
        circle.setPosition(sf::Vector2f(center.x - radius, center.y - radius));
        circle.setFillColor(color);
        if (outline_thickness > 0.0f) {
            circle.setOutlineThickness(outline_thickness);
            circle.setOutlineColor(BACKGROUND);
        }
        window.draw(circle);
    }

    void draw_stick_person(sf::RenderWindow& window, const Pose& pose, 
                          sf::Vector2f center, float song_time, float bpm, 
                          bool turbo_mode, bool mirror_mode, float flash) {
        
        float motion_boost = turbo_mode ? 1.45f : 1.0f;
        float bob = std::sin(song_time * bpm * 3.14159265f / 30.0f) * pose.bob * motion_boost;
        float lean = pose.lean + std::sin(song_time * 2.1f * motion_boost) * (turbo_mode ? 2.3f : 1.5f);
        
        float scale_x = pose.facing * (mirror_mode ? -1.0f : 1.0f);
        
        // Draw flash effect (aura around character)
        if (flash > 0.01f) {
            int aura_radius = static_cast<int>(36 + 25 * flash);
            sf::CircleShape aura(aura_radius);
            aura.setPosition(sf::Vector2f(center.x - aura_radius, center.y - 130 - aura_radius));
            aura.setFillColor(sf::Color::Transparent);
            aura.setOutlineThickness(2.0f);
            aura.setOutlineColor(sf::Color(255, 255, 255, static_cast<int>(255 * flash)));
            window.draw(aura);
        }
        
        // Joint calculation lambda
        auto joint = [&](const Vector2D& offset) -> sf::Vector2f {
            return sf::Vector2f(
                center.x + offset.x * scale_x + lean,
                center.y + offset.y + bob
            );
        };
        
        // Calculate all joint positions
        sf::Vector2f hip = center;
        sf::Vector2f head = joint(pose.head);
        sf::Vector2f neck = joint(pose.neck);
        sf::Vector2f left_elbow = joint(pose.left_elbow);
        sf::Vector2f left_hand = joint(pose.left_hand);
        sf::Vector2f right_elbow = joint(pose.right_elbow);
        sf::Vector2f right_hand = joint(pose.right_hand);
        sf::Vector2f left_knee = joint(pose.left_knee);
        sf::Vector2f left_foot = joint(pose.left_foot);
        sf::Vector2f right_knee = joint(pose.right_knee);
        sf::Vector2f right_foot = joint(pose.right_foot);
        
        // Draw body lines
        draw_line(window, neck, hip, MINT, LINE_WIDTH);
        draw_line(window, neck, left_elbow, PINK, LINE_WIDTH);
        draw_line(window, left_elbow, left_hand, PINK, LINE_WIDTH);
        draw_line(window, neck, right_elbow, YELLOW, LINE_WIDTH);
        draw_line(window, right_elbow, right_hand, YELLOW, LINE_WIDTH);
        draw_line(window, hip, left_knee, BLUE, LINE_WIDTH);
        draw_line(window, left_knee, left_foot, BLUE, LINE_WIDTH);
        draw_line(window, hip, right_knee, MINT, LINE_WIDTH);
        draw_line(window, right_knee, right_foot, MINT, LINE_WIDTH);
        
        // Draw joints
        for (const auto& joint_pos : {neck, left_elbow, left_hand, right_elbow, 
                                       right_hand, hip, left_knee, right_knee}) {
            draw_circle(window, joint_pos, JOINT_RADIUS, WHITE);
        }
        
        // Draw head
        draw_circle(window, head, HEAD_RADIUS, SKIN);
        draw_circle(window, head, HEAD_RADIUS, WHITE, 4.0f);
        
        // Draw eye
        sf::Vector2f eye_pos(head.x + 10 * scale_x, head.y - 5);
        draw_circle(window, eye_pos, 3.0f, BACKGROUND);
        
        // Draw feet
        draw_line(window, left_foot, left_foot + sf::Vector2f(23 * scale_x, 0), WHITE, 7);
        draw_line(window, right_foot, right_foot + sf::Vector2f(23 * scale_x, 0), WHITE, 7);
    }
};

// ============================================================================
// BACKGROUND AND EFFECTS
// ============================================================================

class BackgroundRenderer {
public:
    void draw_background(sf::RenderWindow& window, float song_time, float bpm, 
                        float flash, bool finale_mode) {
        // Clear with background color
        window.clear(BACKGROUND);
        
        // Draw grid
        float pulse = 0.5f + 0.5f * std::sin(song_time * bpm * 3.14159265f / 30.0f);
        int horizon = static_cast<int>(WINDOW_HEIGHT * 0.74f);
        
        // Grid lines
        for (int x = 0; x < WINDOW_WIDTH; x += 64) {
            sf::RectangleShape vline(sf::Vector2f(1, WINDOW_HEIGHT));
            vline.setPosition(sf::Vector2f(x, 0));
            vline.setFillColor(GRID_COLOR);
            window.draw(vline);
        }
        
        for (int y = 0; y < WINDOW_HEIGHT; y += 64) {
            sf::RectangleShape hline(sf::Vector2f(WINDOW_WIDTH, 1));
            hline.setPosition(sf::Vector2f(0, y));
            hline.setFillColor(GRID_COLOR);
            window.draw(hline);
        }
        
        // Stage line and glow
        int stage_glow = static_cast<int>(30 * flash);
        sf::RectangleShape stage_line(sf::Vector2f(WINDOW_WIDTH, 2));
        stage_line.setPosition(sf::Vector2f(0, horizon));
        stage_line.setFillColor(sf::Color(55, 71, 95));
        window.draw(stage_line);
        
        // Stage glow ellipse
        sf::CircleShape glow(170 + static_cast<int>(pulse * 12));
        glow.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 170 - static_cast<int>(pulse * 12), horizon - 12));
        glow.setFillColor(sf::Color(18 + stage_glow, 29 + stage_glow, 43 + stage_glow));
        window.draw(glow);
        
        // Crowd silhouettes
        for (int i = 0; i < WINDOW_WIDTH; i += 16) {
            int person_h = 18 + static_cast<int>(12 * std::sin(i * 0.35f + song_time * 2.0f));
            int person_y = WINDOW_HEIGHT - 110 - person_h;
            sf::CircleShape person(8);
            person.setPosition(sf::Vector2f(i - 8, person_y));
            person.setFillColor(sf::Color(12, 16, 28, 140));
            window.draw(person);
            
            sf::RectangleShape body(sf::Vector2f(2, 8));
            body.setPosition(sf::Vector2f(i - 1, person_y + 10));
            body.setFillColor(sf::Color(18, 24, 38, 200));
            window.draw(body);
        }
        
        // Dynamic lighting
        float light_alpha = 80 + static_cast<int>(35 * std::sin(song_time * bpm * 3.14159265f / 22.0f + 1.2f));
        float center_x = WINDOW_WIDTH / 2.0f;
        float center_y = WINDOW_HEIGHT / 2.0f - 30;
        
        float triad = 0.5f + 0.5f * std::sin(song_time * bpm * 3.14159265f / 16.0f);
        sf::Color left_color(
            static_cast<int>(120 + 135 * triad),
            static_cast<int>(180 + 70 * triad),
            255,
            static_cast<int>(light_alpha)
        );
        sf::Color right_color(
            static_cast<int>(255 - 120 * triad),
            static_cast<int>(110 + 90 * triad),
            static_cast<int>(166 + 80 * triad),
            static_cast<int>(light_alpha)
        );
        
        // Light beams
        for (int band = 0; band <= WINDOW_WIDTH; band += 110) {
            float beam_t = song_time * bpm * 3.14159265f / 36.0f + band / 190.0f;
            int beam_alpha = 35 + static_cast<int>(20 * std::sin(beam_t));
            
            sf::RectangleShape beam(sf::Vector2f(70, WINDOW_HEIGHT * 0.3f));
            beam.setPosition(sf::Vector2f(band, WINDOW_HEIGHT * 0.7f));
            beam.setFillColor(sf::Color(
                static_cast<int>(255 * (0.5f + 0.5f * std::sin(beam_t + 0.5f))),
                static_cast<int>(180 * (0.5f + 0.5f * std::sin(beam_t + 2.1f))),
                static_cast<int>(200 * (0.5f + 0.5f * std::sin(beam_t + 4.1f))),
                beam_alpha
            ));
            window.draw(beam);
        }
        
        // Finale mode sweep effect
        if (finale_mode) {
            float sweep_angle = song_time * 1.2f;
            float sweep_len = WINDOW_WIDTH * 1.2f;
            sf::Vector2f end_pos(
                center_x + std::cos(sweep_angle) * sweep_len,
                center_y + std::sin(sweep_angle) * sweep_len * 0.7f
            );
            
            sf::Vector2f delta = end_pos - sf::Vector2f(center_x, center_y);
            float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            
            sf::RectangleShape sweep(sf::Vector2f(len, 20));
            sweep.setPosition(sf::Vector2f(center_x, center_y));
            sweep.setFillColor(sf::Color(
                static_cast<int>(255 * (0.5f + 0.5f * std::sin(sweep_angle))),
                static_cast<int>(200 * (0.5f + 0.5f * std::sin(sweep_angle + 1.0f))),
                static_cast<int>(255 * (0.5f + 0.5f * std::sin(sweep_angle + 2.0f))),
                110
            ));
            
            sweep.setRotation(sf::degrees(std::atan2(delta.y, delta.x) * 180.0f / 3.14159265f));
            sweep.setOrigin(sf::Vector2f(0, 10));
            window.draw(sweep);
        }
        
        // Flash effect
        if (flash > 0.01f) {
            sf::RectangleShape flash_rect(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            flash_rect.setFillColor(sf::Color::Transparent);
            flash_rect.setOutlineThickness(static_cast<int>(3 * flash));
            flash_rect.setOutlineColor(WHITE);
            window.draw(flash_rect);
        }
    }
};

// ============================================================================
// UI RENDERING
// ============================================================================

class UIRenderer {
public:
    UIRenderer() {
        // Load fonts - using system fonts as fallback
        if (!large_font.openFromFile("C:\\Windows\\Fonts\\Arial.ttf")) {
            std::cerr << "Warning: Could not load Arial font. UI text may not display." << std::endl;
        }
        if (!small_font.openFromFile("C:\\Windows\\Fonts\\Arial.ttf")) {
            std::cerr << "Warning: Could not load Arial font. UI text may not display." << std::endl;
        }
    }

    void draw_ui(sf::RenderWindow& window, float song_time, float beat, float bpm,
                bool turbo_mode, bool mirror_mode, float flash, bool finale_mode) {
        
        int pose_index = static_cast<int>(beat / POSE_BEATS) % POSES.size();
        
        // Title
        sf::Text title(large_font, "STICK SHIFT", 42);
        title.setPosition(sf::Vector2f(34, 28));
        title.setFillColor(WHITE);
        window.draw(title);
        
        // Subtitle
        sf::Text subtitle(small_font, "SIX-DANCER PERFORMANCE", 24);
        subtitle.setPosition(sf::Vector2f(37, 76));
        subtitle.setFillColor(MINT);
        window.draw(subtitle);
        
        // Mode and info
        std::string mode_text = turbo_mode ? "TURBO" : "NORMAL";
        std::string mirror_text = mirror_mode ? "MIRROR" : "STANDARD";
        std::string finale_text = finale_mode ? "FINALE" : "LIVE";
        
        std::stringstream info_ss;
        info_ss << "BPM " << static_cast<int>(bpm) << "   |   " << mode_text 
                << "   |   " << mirror_text << "   |   " << finale_text
                << "   |   DANCERS " << DANCER_COUNT << "   |   MOVE " 
                << (pose_index + 1) << "/" << POSES.size();
        
        sf::Text info(small_font, info_ss.str(), 24);
        info.setPosition(sf::Vector2f(WINDOW_WIDTH - info.getLocalBounds().size.x - 36, 42));
        info.setFillColor(sf::Color(177, 193, 216));
        window.draw(info);
        
        // Time display
        sf::Text time_text(large_font, format_time(song_time), 42);
        time_text.setPosition(sf::Vector2f(WINDOW_WIDTH - time_text.getLocalBounds().size.x - 36, WINDOW_HEIGHT - 74));
        time_text.setFillColor(WHITE);
        window.draw(time_text);
        
        // Controls help
        sf::Text controls(small_font, "SPACE pause   K +10s   T turbo   M mirror   F flash   R restart   ESC quit", 24);
        controls.setPosition(sf::Vector2f(36, WINDOW_HEIGHT - 54));
        controls.setFillColor(sf::Color(177, 193, 216));
        window.draw(controls);
        
        // Beat progress bar
        int beat_width = 180;
        sf::RectangleShape beat_bg(sf::Vector2f(beat_width, 5));
        beat_bg.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - beat_width / 2, 38));
        beat_bg.setFillColor(sf::Color(44, 57, 79));
        window.draw(beat_bg);
        
        float beat_progress = std::fmod(beat, 1.0f);
        sf::RectangleShape beat_fill(sf::Vector2f(beat_width * beat_progress, 5));
        beat_fill.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - beat_width / 2, 38));
        beat_fill.setFillColor(PINK);
        window.draw(beat_fill);
        
        // Flash indicator
        if (flash > 0.01f) {
            sf::Text flash_text(small_font, "FLASH!", 24);
            flash_text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 30, 84));
            flash_text.setFillColor(WHITE);
            window.draw(flash_text);
        }
        
        // Finale indicator
        if (finale_mode) {
            sf::Text finale_text(small_font, "SHOW ENDING", 24);
            finale_text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 52, 104));
            finale_text.setFillColor(YELLOW);
            window.draw(finale_text);
        }
    }

private:
    sf::Font large_font;
    sf::Font small_font;
};

// ============================================================================
// MAIN APPLICATION
// ============================================================================

class StickDanceApp {
public:
    StickDanceApp() : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Stick Shift - Beat-Synchronized Dance"),
                      paused(false), turbo_mode(false), mirror_mode(false), 
                      flash(0.0f), finale_mode(false), song_time(SONG_START_OFFSET) {
        window.setFramerateLimit(FPS);
        
        // Find song file
        std::string song_path = find_song();
        
        if (song_path.empty()) {
            show_missing_song_screen();
            song_file_valid = false;
            return;
        }
        
        // Load music
        if (!music.openFromFile(song_path)) {
            std::cerr << "Error: Could not load audio file: " << song_path << std::endl;
            show_error_screen("Cannot load audio file. Check format (MP3/OGG).");
            song_file_valid = false;
            return;
        }
        
        std::cout << "Loaded song: " << song_path << std::endl;
        song_file_valid = true;
        
        // Start playback
        music.play();
        music.setPlayingOffset(sf::seconds(SONG_START_OFFSET));
        start_time = std::chrono::high_resolution_clock::now();
    }

    bool is_valid() const {
        return song_file_valid;
    }

    void run() {
        if (!is_valid()) return;
        
        sf::Clock clock;
        
        while (window.isOpen()) {
            handle_input();
            
            if (!paused) {
                // Update song time from music playback
                float music_offset = music.getPlayingOffset().asSeconds();
                if (music_offset >= 0) {
                    song_time = music_offset;
                } else {
                    // Fallback to clock-based timing
                    auto now = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
                    song_time = SONG_START_OFFSET + elapsed.count() / 1000.0f;
                }
                
                // Check if song ended
                if (music.getStatus() != sf::SoundSource::Status::Playing && song_time > 0.5f) {
                    break;  // Song finished
                }
            }
            
            // Update effects
            flash = std::max(0.0f, flash - 0.06f);
            finale_mode = song_time > 65.0f;
            
            float effective_bpm = turbo_mode ? TURBO_BPM : BASE_BPM;
            float beat = song_time * effective_bpm / 60.0f;
            
            // Render
            background_renderer.draw_background(window, song_time, effective_bpm, flash, finale_mode);
            
            // Calculate dancer centers
            float spacing = WINDOW_WIDTH / (DANCER_COUNT + 1);
            for (int index = 0; index < DANCER_COUNT; index++) {
                float base_x = spacing * (index + 1);
                float base_y = STAGE_Y + (index % 2) * 6;
                
                float mass_beat = beat + std::sin(song_time * 0.8f + index * 0.7f) * 0.08f;
                float mass_time = song_time + index * 0.04f;
                
                Pose dancer_pose = pose_at_beat(mass_beat, index);
                
                if (finale_mode) {
                    dancer_pose.bob *= 1.5f;
                    dancer_pose.lean *= 1.3f;
                    dancer_pose.facing += std::sin(song_time * 1.4f + index) * 0.12f;
                }
                
                stick_renderer.draw_stick_person(window, dancer_pose, 
                                               sf::Vector2f(base_x, base_y), 
                                               mass_time, effective_bpm,
                                               turbo_mode, mirror_mode, flash);
            }
            
            ui_renderer.draw_ui(window, song_time, beat, effective_bpm,
                              turbo_mode, mirror_mode, flash, finale_mode);
            
            window.display();
        }
        
        music.stop();
    }

private:
    sf::RenderWindow window;
    sf::Music music;
    StickRenderer stick_renderer;
    BackgroundRenderer background_renderer;
    UIRenderer ui_renderer;
    
    bool paused;
    bool turbo_mode;
    bool mirror_mode;
    bool finale_mode;
    bool song_file_valid;
    
    float flash;
    float song_time;
    std::chrono::high_resolution_clock::time_point start_time;

    std::string find_song() {
        // Check for exact filename first
        if (fs::exists(SONG_FILENAME)) {
            return SONG_FILENAME;
        }
        
        // Try to find any MP3 in current directory
        try {
            for (const auto& entry : fs::directory_iterator(".")) {
                std::string ext = entry.path().extension().string();
                // Convert to lowercase for comparison
                std::transform(ext.begin(), ext.end(), ext.begin(),
                    [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                if (ext == ".mp3" || ext == ".ogg") {
                    return entry.path().string();
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error scanning directory: " << e.what() << std::endl;
        }
        
        return "";
    }

    void handle_input() {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                music.stop();
            }
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Escape:
                        window.close();
                        music.stop();
                        break;
                    
                    case sf::Keyboard::Key::Space:
                        toggle_pause();
                        break;
                    
                    case sf::Keyboard::Key::K:
                        song_time = std::min(9999.0f, song_time + SKIP_SECONDS);
                        if (music.getStatus() == sf::SoundSource::Status::Playing) {
                            music.setPlayingOffset(sf::seconds(song_time));
                        }
                        start_time = std::chrono::high_resolution_clock::now() - 
                                    std::chrono::milliseconds(static_cast<int>(song_time * 1000));
                        flash = 1.0f;
                        break;
                    
                    case sf::Keyboard::Key::T:
                        turbo_mode = !turbo_mode;
                        std::cout << (turbo_mode ? "Turbo mode ON" : "Turbo mode OFF") << std::endl;
                        break;
                    
                    case sf::Keyboard::Key::M:
                        mirror_mode = !mirror_mode;
                        std::cout << (mirror_mode ? "Mirror mode ON" : "Mirror mode OFF") << std::endl;
                        break;
                    
                    case sf::Keyboard::Key::F:
                        flash = 1.0f;
                        std::cout << "Flash!" << std::endl;
                        break;
                    
                    case sf::Keyboard::Key::R:
                        music.stop();
                        music.play();
                        music.setPlayingOffset(sf::seconds(SONG_START_OFFSET));
                        song_time = SONG_START_OFFSET;
                        paused = false;
                        flash = 0.0f;
                        start_time = std::chrono::high_resolution_clock::now();
                        std::cout << "Song restarted." << std::endl;
                        break;
                    
                    default:
                        break;
                }
            }
        }
    }

    void toggle_pause() {
        paused = !paused;
        
        if (paused) {
            music.pause();
            std::cout << "PAUSED" << std::endl;
        } else {
            music.play();
            start_time = std::chrono::high_resolution_clock::now() - 
                        std::chrono::milliseconds(static_cast<int>(song_time * 1000));
            std::cout << "RESUMED" << std::endl;
        }
    }

    void show_missing_song_screen() {
        window.clear(BACKGROUND);
        
        sf::Font font;
        font.openFromFile("C:\\Windows\\Fonts\\Arial.ttf");
        
        sf::Text heading(font, "No MP3 song found", 42);
        heading.setFillColor(WHITE);
        heading.setPosition(sf::Vector2f(
            WINDOW_WIDTH / 2 - heading.getLocalBounds().size.x / 2,
            310
        ));
        window.draw(heading);
        
        sf::Text detail(font, "Place an MP3 beside the executable and run again.", 24);
        detail.setFillColor(MINT);
        detail.setPosition(sf::Vector2f(
            WINDOW_WIDTH / 2 - detail.getLocalBounds().size.x / 2,
            365
        ));
        window.draw(detail);
        
        window.display();
        
        // Wait for ESC or window close
        bool waiting = true;
        while (waiting && window.isOpen()) {
            while (const auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>() ||
                    (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {
                    waiting = false;
                    window.close();
                }
            }
            sf::sleep(sf::milliseconds(16));
        }
    }

    void show_error_screen(const std::string& message) {
        window.clear(BACKGROUND);
        
        sf::Font font;
        font.openFromFile("C:\\Windows\\Fonts\\Arial.ttf");
        
        sf::Text heading(font, "Error", 42);
        heading.setFillColor(PINK);
        heading.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - heading.getLocalBounds().size.x / 2, 310));
        window.draw(heading);
        
        sf::Text detail(font, message, 24);
        detail.setFillColor(WHITE);
        detail.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - detail.getLocalBounds().size.x / 2, 365));
        window.draw(detail);
        
        window.display();
        
        sf::sleep(sf::seconds(3));
    }
};

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

int main() {
    try {
        std::cout << "=== Stick Shift: Beat-Synchronized Dance (C++ with SFML) ===" << std::endl;
        std::cout << "Loading application..." << std::endl;
        
        StickDanceApp app;
        
        if (!app.is_valid()) {
            std::cerr << "Application initialization failed." << std::endl;
            return 1;
        }
        
        std::cout << "Starting animation loop..." << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  SPACE = Pause/Resume" << std::endl;
        std::cout << "  K     = Skip +10 seconds" << std::endl;
        std::cout << "  T     = Toggle Turbo mode" << std::endl;
        std::cout << "  M     = Toggle Mirror mode" << std::endl;
        std::cout << "  F     = Flash effect" << std::endl;
        std::cout << "  R     = Restart song" << std::endl;
        std::cout << "  ESC   = Quit" << std::endl;
        
        app.run();
        
        std::cout << "Application closed gracefully." << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
