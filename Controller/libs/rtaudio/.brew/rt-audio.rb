class RtAudio < Formula
  desc "API for realtime audio input/output"
  homepage "https://www.music.mcgill.ca/~gary/rtaudio/"
  url "https://www.music.mcgill.ca/~gary/rtaudio/release/rtaudio-5.1.0.tar.gz"
  sha256 "ff138b2b6ed2b700b04b406be718df213052d4c952190280cf4e2fab4b61fe09"
  head "https://github.com/thestk/rtaudio.git"

  def install
    system "./configure", "--disable-debug",
                          "--disable-dependency-tracking",
                          "--disable-silent-rules",
                          "--prefix=#{prefix}"
    system "make", "install"
    doc.install Dir["doc/*"]
    pkgshare.install "tests"
  end

  test do
    system ENV.cxx, "-I#{include}/rtaudio", "-L#{lib}", "-lrtaudio",
           pkgshare/"tests/testall.cpp", "-o", "test"
    system "./test"
  end
end
