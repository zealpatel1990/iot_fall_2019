let fft;

function preload() {
  sound = loadSound('cat_6.mp3');
}

function setup() {
  let cnv = createCanvas(710, 200);
  
  fft = new p5.FFT();
  sound.amp(0.2);
  sound.loop();
}
function draw(){
  background(0);

  let spectrum = fft.analyze();
  noStroke();
  fill(0,255,0); // spectrum is green
  for (var i = 0; i< spectrum.length; i++){
    let x = map(i, 0, spectrum.length, 0, width);
    let h = -height + map(spectrum[i], 0, 255, height, 0);
    rect(x, height, width / spectrum.length, h )
  }

}
