extern crate sdl2;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::render::WindowCanvas;

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
        .allow_highdpi()
        .resizable()
        .build()
        .unwrap();

    let mut canvas = window.into_canvas().build().unwrap();

    let mut timer = sdl_context.timer().expect("Init of timer failed");

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
        move_speed: 0.008f64,
        rot_speed: 0.005f64,
    };
    let mut last = 0;
    loop {
        let ticks = timer.ticks();
        let delta_time = ticks - last;
        last = ticks;
        player = match handle_input(player, &sdl_context, &world_map, delta_time) {
            Ok(p) => p,
            Err(_) => break,
        };
        canvas = render(canvas, &player, &world_map);
    }

    println!("Exiting");
}

fn render(mut canvas: WindowCanvas, player: &Player, world_map: &Vec<Vec<u8>>) -> WindowCanvas {
    use sdl2::rect::Point;
    use sdl2::rect::Rect;
    canvas.set_draw_color(Color::RGB(102, 103, 123));
    canvas
        .fill_rect(Rect::new(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2))
        .expect("An error occurred for toprect");
    canvas.set_draw_color(Color::RGB(255, 255, 255));
    canvas
        .fill_rect(Rect::new(
            0,
            (SCREEN_HEIGHT / 2) as i32,
            SCREEN_WIDTH,
            SCREEN_HEIGHT / 2,
        ))
        .expect("And error ocurred for bottom rect");
    for x in 0..SCREEN_WIDTH {
        let camera_x = 2f64 * x as f64 / SCREEN_WIDTH as f64 - 1f64;
        let ray_pos_x = player.position.x;
        let ray_pos_y = player.position.y;
        let ray_direction = Vector2D {
            x: player.direction.x + player.plane.x * camera_x,
            y: player.direction.y + player.plane.y * camera_x,
        };

        let mut map_x = ray_pos_x.floor();
        let mut map_y = ray_pos_y.floor();

        let delta_dist_x = (1.0f64
            + (ray_direction.y * ray_direction.y) / (ray_direction.x * ray_direction.x))
            .sqrt();
        let delta_dist_y = (1.0f64
            + (ray_direction.x * ray_direction.x) / (ray_direction.y * ray_direction.y))
            .sqrt();

        let mut hit = false;

        let step_x;
        let step_y;
        let mut side_dist_x;
        let mut side_dist_y;

        if ray_direction.x < 0f64 {
            step_x = -1f64;
            side_dist_x = (ray_pos_x - map_x) * delta_dist_x;
        } else {
            step_x = 1f64;
            side_dist_x = (map_x + 1.0f64 - ray_pos_x) * delta_dist_x;
        }

        if ray_direction.y < 0f64 {
            step_y = -1f64;
            side_dist_y = (ray_pos_y - map_y) * delta_dist_y;
        } else {
            step_y = 1f64;
            side_dist_y = (map_y + 1f64 - ray_pos_y) * delta_dist_y;
        }

        let mut side = -1;
        let perp_wall_distance;

        while !hit {
            if side_dist_x < side_dist_y {
                side_dist_x = side_dist_x + delta_dist_x;
                map_x = map_x + step_x;
                side = 0;
            } else {
                side_dist_y = side_dist_y + delta_dist_y;
                map_y = map_y + step_y;
                side = 1;
            }
            if world_map[map_x as usize][map_y as usize] > 0 {
                hit = true;
            }
        }

        if side == 0 {
            perp_wall_distance = (map_x - ray_pos_x + (1f64 - step_x) / 2f64) / ray_direction.x;
        } else {
            perp_wall_distance = (map_y - ray_pos_y + (1f64 - step_y) / 2f64) / ray_direction.y;
        }

        let line_height = (SCREEN_HEIGHT as f64 / perp_wall_distance).floor();

        let mut draw_start: i32 = (-line_height / 2f64 + (SCREEN_HEIGHT as f64 / 2f64)) as i32;
        if draw_start < 0 {
            draw_start = 0;
        }
        let mut draw_end: u32 = (line_height / 2f64 + SCREEN_HEIGHT as f64 / 2f64) as u32;
        if draw_end >= SCREEN_HEIGHT {
            draw_end = SCREEN_HEIGHT - 1;
        }

        let mut map_value: u8 = world_map[map_x as usize][map_y as usize];

        if side == 1 {
            map_value = (map_value as f64 / 2f64) as u8;
        }

        match map_value {
            0 => canvas.set_draw_color(Color::RGB(255, 255, 0)),
            1 => canvas.set_draw_color(Color::RGB(255, 0, 0)),
            2 => canvas.set_draw_color(Color::RGB(0, 255, 0)),
            3 => canvas.set_draw_color(Color::RGB(0, 0, 255)),
            _ => canvas.set_draw_color(Color::RGB(0, 0, 0)),
        }

        canvas
            .draw_line(
                Point::new(x as i32, draw_end as i32),
                Point::new(x as i32, draw_start),
            )
            .expect("Drawing line went wrong");
    }
    canvas.present();
    canvas.clear();
    canvas
}

fn handle_input(
    mut player: Player,
    sdl_context: &sdl2::Sdl,
    world_map: &Vec<Vec<u8>>,
    delta_time: u32,
) -> Result<Player, bool> {
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
    if exit {
        return Err(true);
    }
    for _i in 0..delta_time {
        if player.up {
            println!(
                "{}",
                (player.position.x + player.direction.x * player.move_speed)
            );
            if world_map[(player.position.x + player.direction.x * player.move_speed) as usize]
                [(player.position.y) as usize]
                == 0
            {
                player.position.x = player.position.x + player.direction.x * player.move_speed;
            }
            if world_map[(player.position.x) as usize]
                [(player.position.y + player.direction.y * player.move_speed) as usize]
                == 0
            {
                player.position.y = player.position.y + player.direction.y * player.move_speed;
            }
        }

        if player.right == true {
            player = turn(player, TurnDirection::RIGHT, delta_time);
        }
        if player.left {
            player = turn(player, TurnDirection::LEFT, delta_time);
        }
    }
    Ok(player)
}

enum TurnDirection {
    LEFT,
    RIGHT,
}

fn turn(mut player: Player, dir: TurnDirection, delta_time: u32) -> Player {
    let rotation = match dir {
        TurnDirection::RIGHT => -player.rot_speed,
        TurnDirection::LEFT => player.rot_speed,
    };
    for _i in 0..delta_time {
        let old_dir_x = player.direction.x;
        let old_plane_x = player.plane.x;
        player.direction.x =
            player.direction.x * rotation.cos() - player.direction.y * rotation.sin();
        player.direction.y = old_dir_x * rotation.sin() + player.direction.y * rotation.cos();
        player.plane.x = player.plane.x * rotation.cos() - player.plane.y * rotation.sin();
        player.plane.y = old_plane_x * rotation.sin() + player.plane.y * rotation.cos();
    }
    player
}
