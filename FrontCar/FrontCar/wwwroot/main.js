function canGame() {
    return "getGamepads" in navigator;
}

$(document).ready(function () {

    var sHelp = new socketHelp();

    sHelp.connect();







    var canvas = document.getElementById('canvas');
    var ctx = canvas.getContext('2d');
    if (canGame()) {

        var prompt = "To begin using your gamepad, connect it and press any button!";
        $("#gamepadPrompt").text(prompt);

        $(window).on("gamepadconnected", function (x) {
            $("#gamepadPrompt").html("Gamepad connected!");
            console.log("connection event");
        });
        setInterval(function () {
            var gp = navigator.getGamepads()[0];
            /*console.log(navigator.getGamepads()[2]);
            return;*/
            var black = 255 - (Math.round(gp.buttons[7].value * 255));
            var blue = black - (Math.round(gp.buttons[6].value * 255));
            if (blue < 0) {
                blue = 0;
            }

            //console.log("rgba(" + blue + "," + blue + "," + black + ", 1)")

            ctx.fillStyle = "rgba(" + blue + "," + blue + "," + black + ", 1)";
            ctx.fillRect(0, 0, 1010, 1010);
            ctx.fillStyle = 'green';
            ctx.fillRect((gp.axes[0] + 1) * 500, (gp.axes[1] + 1) * 500, 10, 10);
            ctx.fillStyle = 'red';
            ctx.fillRect((gp.axes[2] + 1) * 500, (gp.axes[3] + 1) * 500, 10, 10);
            var cd = {
                LeftPad: { x: Math.round(gp.axes[0] * 100), y: Math.round(gp.axes[1] * 100) },
                RightPad: { x: Math.round(gp.axes[2] * 100), y: Math.round(gp.axes[3] * 100) },
                LeftTrigger: Math.round(gp.buttons[6].value * 100),
                RightTrigger: Math.round(gp.buttons[7].value * 100),
                StearingCors: {
                    Upp: gp.buttons[12].value,
                    Down: gp.buttons[13].value,
                    Left: gp.buttons[14].value,
                    Right: gp.buttons[15].value
                }
            }
            
            console.log("Steering: ", cd.LeftPad.x, "Throttle",cd.RightTrigger-cd.LeftTrigger);
            sHelp.sendData(JSON.stringify(cd));

        }, 100)

        $(window).on("gamepaddisconnected", function () {
            console.log("disconnection event");
            $("#gamepadPrompt").text(prompt);
        });

    }





});