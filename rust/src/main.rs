use raymond_code_golf::Pixel;

fn main() {
    let darkness = std::env::args().skip(1).take(1).next().map(|s| s.parse().unwrap()).unwrap_or(8);

    let buf = raymond_code_golf::random_image(125);

    let mut buf1 = buf.clone();
    raymond_code_golf::v1_darken(&mut buf1, darkness);

    check(&buf, &buf1, darkness, raymond_code_golf::v2_darken);
    check(&buf, &buf1, darkness, raymond_code_golf::v3::darken);
    check(&buf, &buf1, darkness, raymond_code_golf::v4::darken);
    check(&buf, &buf1, darkness, raymond_code_golf::v5d::darken);
    check(&buf, &buf1, darkness, raymond_code_golf::v6d::darken);
    check(&buf, &buf1, darkness, raymond_code_golf::v7d::darken);
}

fn check<F>(src: &Vec<Pixel>, dst: &[Pixel], darkness: i32, darken: F) where F: Fn(&mut [Pixel], i32) {
    let mut buf = src.clone();
    darken(&mut buf, darkness);
    assert_eq!(buf, dst);
}
