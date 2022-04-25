#![feature(portable_simd)]
#![feature(slice_as_chunks)]
#![feature(test)]
extern crate test;

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Pixel(pub [u8; 4]);

pub fn random_image(size: usize) -> Vec<Pixel> {
    let mut rng = rand::thread_rng();
    let mut image = vec![Pixel([0; 4]); size];
    let ptr = image.as_mut_ptr() as *mut u8;
    let len = image.len() * std::mem::size_of::<Pixel>();
    let bytes = unsafe { std::slice::from_raw_parts_mut(ptr, len) };
    rand::Rng::fill(&mut rng, bytes);
    image
}

pub fn v1_darken(buf: &mut [Pixel], darkness: i32) {
    let lightness = 256 - darkness;
    for p in buf {
        for i in 0..3 {
            p.0[i] = (p.0[i] as i32 * lightness / 256) as u8;
        }
    }
}

#[bench]
fn bench_v1(b: &mut test::Bencher) {
    let mut buf = random_image(test::black_box(1_000_000));
    let darkness = test::black_box(8);
    b.iter(|| v1_darken(&mut buf, darkness));
}

pub fn v2_darken(buf: &mut [Pixel], darkness: i32) {
    let lightness = 256 - darkness;
    for p in buf {
        p.0[0] = (p.0[0] as i32 * lightness / 256) as u8;
        p.0[1] = (p.0[1] as i32 * lightness / 256) as u8;
        p.0[2] = (p.0[2] as i32 * lightness / 256) as u8;
    }
}

#[bench]
fn bench_v2(b: &mut test::Bencher) {
    let mut buf = random_image(test::black_box(1_000_000));
    let darkness = test::black_box(8);
    b.iter(|| v2_darken(&mut buf, darkness));
}

pub mod v3;
pub mod v4;
pub mod v5d;
pub mod v6d;
pub mod v7d;
pub mod v8d;
pub mod v9d;
