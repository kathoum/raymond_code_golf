use crate::Pixel;

pub fn darken(buf: &mut [Pixel], darkness: i32) {
    let lightness = (256 - darkness) as u32;
    for p in buf {
        let Pixel([r, g, b, a]) = *p;
        let v = Pixel([
            (r as u32 * lightness / 256) as u8,
            (g as u32 * lightness / 256) as u8,
            (b as u32 * lightness / 256) as u8,
            a
        ]);
        *p = v;
    }
}

#[bench]
fn bench(b: &mut test::Bencher) {
    let mut buf = crate::random_image(test::black_box(1_000_000));
    let darkness = test::black_box(8);
    b.iter(|| darken(&mut buf, darkness));
}
