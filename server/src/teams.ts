import { Router } from "express";
import sqlite3 from "sqlite3";
import { open } from "sqlite";

interface Team {
  id: string;
  name: string;
  deviceIp?: string;
}

const teamsRouter = Router();

// Initialize SQLite database
const initDb = async () => {
  const db = await open({
    filename: "./teams.db",
    driver: sqlite3.Database,
  });

  await db.exec(`
        CREATE TABLE IF NOT EXISTS teams (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL, 
            deviceIp TEXT
        )
    `);

  return db;
};

let dbPromise = initDb();

// Get all teams
teamsRouter.get("/", async (req, res) => {
  try {
    const db = await dbPromise;
    const teams = await db.all("SELECT * FROM teams");
    res.json(teams);
  } catch (error) {
    res.status(500).json({ error: "Database error" });
  }
});

// Get team by id
teamsRouter.get("/:id", async (req, res) => {
  try {
    const db = await dbPromise;
    const team = await db.get(
      "SELECT * FROM teams WHERE id = ?",
      req.params.id
    );
    if (!team) {
      return res.status(404).json({ error: "Team not found" });
    }
    res.json(team);
  } catch (error) {
    res.status(500).json({ error: "Database error" });
  }
});

// Create new team
teamsRouter.post("/", async (req, res) => {
  const { name } = req.body;

  if (!name) {
    return res.status(400).json({ error: "Name is required" });
  }

  const team: Team = {
    id: Date.now().toString(),
    name,
    deviceIp: req.body.deviceIp,
  };

  try {
    const db = await dbPromise;
    await db.run(
      "INSERT INTO teams (id, name, deviceIp) VALUES (?, ?, ?)",
      team.id,
      team.name,
      team.deviceIp
    );
    res.status(201).json(team);
  } catch (error) {
    res.status(500).json({ error: "Database error" });
  }
});

// Update team
teamsRouter.put("/:id", async (req, res) => {
  const { name, deviceIp } = req.body;

  try {
    const db = await dbPromise;
    const result = await db.run(
      "UPDATE teams SET name = ?, deviceIp = ? WHERE id = ?",
      name,
      deviceIp,
      req.params.id
    );

    if (result.changes === 0) {
      return res.status(404).json({ error: "Team not found" });
    }

    const team = await db.get(
      "SELECT * FROM teams WHERE id = ?",
      req.params.id
    );
    res.json(team);
  } catch (error) {
    res.status(500).json({ error: "Database error" });
  }
});

// Delete team
teamsRouter.delete("/:id", async (req, res) => {
  try {
    const db = await dbPromise;
    const result = await db.run(
      "DELETE FROM teams WHERE id = ?",
      req.params.id
    );

    if (result.changes === 0) {
      return res.status(404).json({ error: "Team not found" });
    }

    res.status(204).send();
  } catch (error) {
    res.status(500).json({ error: "Database error" });
  }
});

export default teamsRouter;
