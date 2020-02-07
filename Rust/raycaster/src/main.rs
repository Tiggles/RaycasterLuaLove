extern crate sdl2;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::render::{Canvas, WindowCanvas};
use sdl2::video::Window;
use std::time::Duration;

struct Vector2D {
    x: f64,
    y: f64,
}

struct Player {
    position: Vector2D,
    direction: Vector2D,
    plane: Vector2D,
    up: bool,
    down: bool,
    left: bool,
    right: bool,
    move_speed: f64,
    rot_speed: f64,
}

const SCREEN_HEIGHT: u32 = 600;
const SCREEN_WIDTH: u32 = 800;

fn main() {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().expect("Failed getting video subsystem");

    let window = video_subsystem
        .window("Raycaster", SCREEN_WIDTH as u32, SCREEN_HEIGHT as u32)
        .position_centered()
        .resizable()
        .allow_highdpi()
        .build()
        .unwrap();

    let mut canvas = window.into_canvas().build().unwrap();

    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();

    let world_map: Vec<Vec<u8>> = vec![
        vec![
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        ],
        vec![
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        ],
    ];

    let direction = Vector2D {
        x: -1.0f64,
        y: 0.0f64,
    };

    let position = Vector2D {
        x: 21.0f64,
        y: 11.0f64,
    };

    let plane = Vector2D {
        x: 0.0f64,
        y: 0.66f64,
    };

    let mut player = Player {
        direction,
        position,
        plane,
        up: false,
        down: false,
        left: false,
        right: false,
        move_speed: 0.02f64,
        rot_speed: 0.04f64,
    };

    loop {
        let delta_time = 0;
        let last = 0;
        player = match handle_input(player, &sdl_context) {
            Ok(p) => p,
            Err(_) => break,
        };
        canvas = render(canvas);
    }

    println!("Exiting");
}

fn render(mut canvas: WindowCanvas) -> WindowCanvas {
    use sdl2::rect::Point;
    let mut iteration: u8 = 0;
    for i in 0..SCREEN_WIDTH {
        iteration = (iteration + 1) % 255;
        canvas.set_draw_color(Color::RGB(iteration as u8, 64, 255 - iteration));
        canvas
            .draw_line(
                Point::new(i as i32, 0),
                Point::new(i as i32, SCREEN_HEIGHT as i32),
            )
            .expect("Something went wrong drawing vertical lines");
    }
    canvas.present();
    canvas
}

fn handle_input(mut player: Player, sdl_context: &sdl2::Sdl) -> Result<Player, bool> {
    let mut event_pump = sdl_context.event_pump().unwrap();
    let mut exit = false;
    for event in event_pump.poll_iter() {
        match event {
            Event::Quit { .. }
            | Event::KeyDown {
                keycode: Some(Keycode::Escape),
                ..
            } => exit = true,
            Event::KeyDown {
                keycode: Some(key), ..
            } => match key {
                Keycode::Up => player.up = true,
                Keycode::Down => player.down = true,
                Keycode::Left => player.left = true,
                Keycode::Right => player.right = true,
                _ => println!("Pressed {}", key),
            },
            Event::KeyUp {
                keycode: Some(key), ..
            } => match key {
                Keycode::Up => player.up = false,
                Keycode::Down => player.down = false,
                Keycode::Left => player.left = false,
                Keycode::Right => player.right = false,
                _ => println!("Released {}", key),
            },
            _ => {}
        }
    }
    println!("{}", player.up);
    println!("{}", player.down);
    println!("{}", player.left);
    println!("{}", player.right);
    if exit {
        Err(true)
    } else {
        Ok(player)
    }
}
