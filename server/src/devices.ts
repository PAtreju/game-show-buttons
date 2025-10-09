import { Router } from "express";
import { WebSocket } from "ws";

export const devices = new Map<string, WebSocket>();
let pressedButton = null as string | null;

export function setPressedButton(button: string | null) {
  pressedButton = button;
}

export function clearPressedButton() {
  pressedButton = null;
}

export function getPressedButton() {
  return pressedButton;
}

const devicesRouter = Router();

devicesRouter.get("/", (req, res) => {
  res.json(
    Array.from(devices.keys()).filter(
      (ip) => devices.get(ip)?.readyState === WebSocket.OPEN
    )
  );
});

devicesRouter.get("/pressed", (req, res) => {
  res.json({ pressedButton });
});

export { devicesRouter };
