# Game Show Buttons - Web Interface

A React-based web interface for managing teams and monitoring device connections in the game show button system.

## Features

- **Team Management**: Create, edit, and delete teams
- **Device Assignment**: Assign device IP addresses to teams
- **Real-time Status**: Monitor connected devices and server status
- **Live Updates**: WebSocket integration for instant button press notifications
- **Game Control**: Reset button states and monitor active presses
- **Modern UI**: Built with shadcn/ui components and Tailwind CSS

## Setup

1. Install dependencies:

   ```bash
   npm install
   ```

2. Start the development server:

   ```bash
   npm run dev
   ```

3. Make sure the backend server is running on `http://localhost:3000`

## API Integration

The web interface connects to the backend server to:

- Manage teams (CRUD operations)
- Monitor connected devices
- Real-time updates via REST API
- **Live WebSocket updates for:**
  - Device connection/disconnection events
  - Button press notifications
  - Game reset events
  - Real-time device status changes

## Components

- **TeamsList**: Main component for team management with table view and live button press indicators
- **TeamForm**: Dialog form for creating/editing teams
- **GameStatus**: Live status panel showing WebSocket connection, pressed buttons, and device monitoring
- **useTeams**: Custom hook for team data management
- **useWebSocket**: WebSocket context and hook for real-time updates

## WebSocket Features

The application automatically connects to the WebSocket server and provides:

- **Live Button Press Detection**: Teams table highlights when a button is pressed
- **Real-time Device Status**: Instant updates when devices connect/disconnect
- **Game Reset Control**: Reset all button states from the web interface
- **Connection Monitoring**: Visual indicators for WebSocket connection status

## Development

The app uses Vite proxy configuration to handle API calls during development. In production, update the `API_BASE_URL` in `src/lib/api.ts` to point to your production server.

The React Compiler is not enabled on this template because of its impact on dev & build performances. To add it, see [this documentation](https://react.dev/learn/react-compiler/installation).

## Expanding the ESLint configuration

If you are developing a production application, we recommend updating the configuration to enable type-aware lint rules:

```js
export default defineConfig([
  globalIgnores(["dist"]),
  {
    files: ["**/*.{ts,tsx}"],
    extends: [
      // Other configs...

      // Remove tseslint.configs.recommended and replace with this
      tseslint.configs.recommendedTypeChecked,
      // Alternatively, use this for stricter rules
      tseslint.configs.strictTypeChecked,
      // Optionally, add this for stylistic rules
      tseslint.configs.stylisticTypeChecked,

      // Other configs...
    ],
    languageOptions: {
      parserOptions: {
        project: ["./tsconfig.node.json", "./tsconfig.app.json"],
        tsconfigRootDir: import.meta.dirname,
      },
      // other options...
    },
  },
]);
```

You can also install [eslint-plugin-react-x](https://github.com/Rel1cx/eslint-react/tree/main/packages/plugins/eslint-plugin-react-x) and [eslint-plugin-react-dom](https://github.com/Rel1cx/eslint-react/tree/main/packages/plugins/eslint-plugin-react-dom) for React-specific lint rules:

```js
// eslint.config.js
import reactX from "eslint-plugin-react-x";
import reactDom from "eslint-plugin-react-dom";

export default defineConfig([
  globalIgnores(["dist"]),
  {
    files: ["**/*.{ts,tsx}"],
    extends: [
      // Other configs...
      // Enable lint rules for React
      reactX.configs["recommended-typescript"],
      // Enable lint rules for React DOM
      reactDom.configs.recommended,
    ],
    languageOptions: {
      parserOptions: {
        project: ["./tsconfig.node.json", "./tsconfig.app.json"],
        tsconfigRootDir: import.meta.dirname,
      },
      // other options...
    },
  },
]);
```
