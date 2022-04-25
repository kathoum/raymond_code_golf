use crate::Pixel;

pub fn darken(buf: &mut [Pixel], darkness: i32) {
    let (prefix, buf, suffix) = unsafe { buf.align_to_mut::<u32>() };
    assert_eq!(prefix.len(), 0);
    assert_eq!(suffix.len(), 0);

    let factor = (darkness / 8) as u32;
    for p in buf {
        let mut v = *p;
        let mut fields = (v & 0xff) | ((v & 0xff_00) << 2) | ((v & 0xff_00_00) << 4);
        fields *= factor;
        fields += 0b11111_00000_11111_00000_11111;
        v -= (fields >> 5) & 0x1f;
        v -= (fields >> 7) & 0x1f_00;
        v -= (fields >> 9) & 0x1f_00_00;
        *p = v;
    }
}

#[bench]
fn bench(b: &mut test::Bencher) {
    let mut buf = crate::random_image(test::black_box(1_000_000));
    let darkness = test::black_box(8);
    b.iter(|| darken(&mut buf, darkness));
}
