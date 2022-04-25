use crate::Pixel;
use core::simd::*;
use core::arch::x86_64::*;

pub fn darken(buf: &mut [Pixel], darkness: i32) {
    let lightness = (256 - darkness) as u64;
    let multiplier = lightness * 0x0001_0001_0001 + 0x0100_0000_0000_0000;
    let multiplier = u16x16::from(__m256i::from(u64x4::splat(multiplier)));

    let (chunks, remainder) = buf.as_chunks_mut::<8>();
    for buf in chunks {
        let ptr = buf.as_mut_ptr() as *mut u8x32;
        let p = unsafe { std::ptr::read_unaligned(ptr) };

        let (p1, p2) = p.interleave(u8x32::splat(0));

        let p1 = u16x16::from(__m256i::from(p1));
        let p1 = p1 * multiplier;
        let p1 = u8x32::from(__m256i::from(p1));

        let p2 = u16x16::from(__m256i::from(p2));
        let p2 = p2 * multiplier;
        let p2 = u8x32::from(__m256i::from(p2));

        let (_, q) = p1.deinterleave(p2);

        unsafe { std::ptr::write_unaligned(ptr, q) };
    }
    crate::v1_darken(remainder, darkness);
}

#[bench]
fn bench(b: &mut test::Bencher) {
    let mut buf = crate::random_image(test::black_box(1_000_000));
    let darkness = test::black_box(8);
    b.iter(|| darken(&mut buf, darkness));
}
