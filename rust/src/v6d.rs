use crate::Pixel;

pub fn darken(buf: &mut [Pixel], darkness: i32) {
    let (prefix, buf, suffix) = unsafe { buf.align_to_mut::<u32>() };
    assert_eq!(prefix.len(), 0);
    assert_eq!(suffix.len(), 0);

    let lightness = (256 - darkness) as u32;
    for p in buf {
        let v = *p;
        let r = ((v & 0xff) * lightness / 256) & 0xff;
        let g = ((v & 0xff_00) * lightness / 256) & 0xff_00;
        let b = ((v & 0xff_00_00) * lightness / 256) & 0xff_00_00;
        let a = v & 0xff_00_00_00;
        *p = r | g | b | a;
    }
}

#[bench]
fn bench(b: &mut test::Bencher) {
    let mut buf = crate::random_image(test::black_box(1_000_000));
    let darkness = test::black_box(8);
    b.iter(|| darken(&mut buf, darkness));
}
