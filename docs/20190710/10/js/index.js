
var wrapper = document.querySelector('.thamb-wrapper'),
    roof = document.querySelectorAll('.block__front'),
    wall = document.querySelectorAll('.wall'),
    home = document.querySelector('.home'),
    houseWrapper = document.querySelector('.home-wrapper'),
    fronton = document.querySelector('.fronton'),
    coord = void 0;

function setPattern(plates, targetBackground) {
	Array.from(plates).forEach(function (v) {
		return v.style.backgroundImage = targetBackground.style.backgroundImage;
	});
}

wrapper.addEventListener('click', function (e) {
	var targSt = e.target;
	if (targSt.classList.contains('t-roof')) {
		setPattern(roof, targSt);
	} else if (targSt.classList.contains('t-wall')) {
		setPattern(wall, targSt);
	} else if (targSt.classList.contains('t-fronton')) {
		fronton.style.borderBottomColor = targSt.style.backgroundColor;
	}
}, false);

houseWrapper.addEventListener('mousemove', function (e) {
	coord = { x: e.offsetX, y: e.offsetY };
	home.style.transform = 'rotateY(' + coord.x / 5 + 'deg) rotateX(' + -coord.y / 20 + 'deg) translate(-50%, -50%)';
});