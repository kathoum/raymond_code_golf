use crate::Pixel;

fn pack_fields(r: u8, g: u8, b: u8) -> u32 {
    (r as u32) | ((g as u32) << 10) | ((b as u32) << 20)
}

pub fn darken(buf: &mut [Pixel], darkness: i32) {
    let factor = (darkness / 8) as u32;
    for p in buf {
        let mut fields = pack_fields(p.0[0], p.0[1], p.0[2]);
        fields *= factor;
        fields += pack_fields(31, 31, 31);
        p.0[0] -= ((fields >>  5) & 31) as u8;
        p.0[1] -= ((fields >> 15) & 31) as u8;
        p.0[2] -= ((fields >> 25) & 31) as u8;
    }
}

#[bench]
fn bench(b: &mut test::Bencher) {
    let mut buf = crate::random_image(test::black_box(1_000_000));
    let darkness = test::black_box(8);
    b.iter(|| darken(&mut buf, darkness));
}
