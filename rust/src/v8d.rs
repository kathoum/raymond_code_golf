use crate::Pixel;
use core::simd::*;

pub fn darken(buf: &mut [Pixel], darkness: i32) {
    let l = (256 - darkness) as u16;
    let mult = u16x16::from_array([l, l, l, 256, l, l, l, 256, l, l, l, 256, l, l, l, 256]);

    let (chunks, remainder) = buf.as_chunks_mut::<4>();
    for buf in chunks {
        let ptr = buf.as_mut_ptr() as *mut u8x16;
        let p = unsafe { std::ptr::read_unaligned(ptr) };
        let q = (p.cast::<u16>() * mult) >> u16x16::splat(8);
        let r = q.cast::<u8>();

        unsafe { std::ptr::write_unaligned(ptr, r) };
    }
    crate::v1_darken(remainder, darkness);
}

#[bench]
fn bench(b: &mut test::Bencher) {
    let mut buf = crate::random_image(test::black_box(1_000_000));
    let darkness = test::black_box(8);
    b.iter(|| darken(&mut buf, darkness));
}
