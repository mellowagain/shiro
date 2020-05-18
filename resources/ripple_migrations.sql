-- SHIRO SQL MIGRATIONS
-- Converts Ripple database data to Shiro database data
-- The following items are getting converted:
-- * Users
-- * User Stats
-- * User Relationships
-- * Scores
-- Shiro or Kuro needs to have run at least once before in order to create the empty tables.
-- This script will both copy (stats, relationships) and move (users, scores) so MAKE SURE YOU HAVE A BACKUP as RUNNING THIS IS IRREVERSIBLE!

-- Users table
DROP TABLE shiro.users;
RENAME TABLE ripple.users TO shiro.users;
ALTER TABLE shiro.users MODIFY id INT(11) PRIMARY KEY NOT NULL AUTO_INCREMENT;
ALTER TABLE shiro.users MODIFY username VARCHAR(32) NOT NULL;
ALTER TABLE shiro.users CHANGE COLUMN username_safe safe_username VARCHAR(32) NOT NULL;
ALTER TABLE shiro.users DROP COLUMN notes;
ALTER TABLE shiro.users DROP COLUMN ban_datetime;
ALTER TABLE shiro.users DROP COLUMN password_md5; -- Shiro uses a different password hashing algorithm than Ripple; force password resets by making every password empty
ALTER TABLE shiro.users MODIFY salt VARCHAR(64) NOT NULL;
ALTER TABLE shiro.users MODIFY email VARCHAR(100) NOT NULL;
ALTER TABLE shiro.users CHANGE COLUMN register_datetime registration_date INT(11) NOT NULL; 
ALTER TABLE shiro.users DROP COLUMN rank;
ALTER TABLE shiro.users DROP COLUMN allowed;
ALTER TABLE shiro.users CHANGE COLUMN latest_activity last_seen INT(11) NOT NULL; 
ALTER TABLE shiro.users DROP COLUMN silence_end;
ALTER TABLE shiro.users DROP COLUMN silence_reason;
ALTER TABLE shiro.users DROP COLUMN password_version;
ALTER TABLE shiro.users CHANGE COLUMN privileges roles INT(10) UNSIGNED NOT NULL; 
ALTER TABLE shiro.users DROP COLUMN donor_expire;
ALTER TABLE shiro.users DROP COLUMN flags;
ALTER TABLE shiro.users DROP COLUMN achievements_version;
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (password VARCHAR(128) NOT NULL);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (ip VARCHAR(39) NOT NULL);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (followers INT(11) NOT NULL);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (user_page TEXT NOT NULL);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (pp_std FLOAT NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (pp_taiko FLOAT NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (pp_ctb FLOAT NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (pp_mania FLOAT NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (rank_std INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (rank_taiko INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (rank_ctb INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (rank_mania INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (score_std INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (score_taiko INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (score_ctb INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (score_mania INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (ranked_score_std INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (ranked_score_taiko INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (ranked_score_ctb INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (ranked_score_mania INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (accuracy_std FLOAT NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (accuracy_taiko FLOAT NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (accuracy_ctb FLOAT NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (accuracy_mania FLOAT NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (play_count_std INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (play_count_taiko INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (play_count_ctb INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (play_count_mania INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (max_combo_std INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (max_combo_taiko INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (max_combo_ctb INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (max_combo_mania INT(11) NOT NULL DEFAULT 0);
ALTER TABLE shiro.users ADD COLUMN IF NOT EXISTS (country VARCHAR(2) NOT NULL);

-- User stats
-- This does not import max_combo and rank; these need to be recalculated by Shiro manually (in-game command: !recalculate)
UPDATE shiro.users JOIN ripple.users_stats r ON r.id = shiro.users.id
	SET shiro.users.pp_std = r.pp_std,
		shiro.users.pp_taiko = r.pp_taiko,
		shiro.users.pp_ctb = r.pp_ctb,
		shiro.users.pp_mania = r.pp_mania,
		shiro.users.score_std = r.total_score_std,
		shiro.users.score_taiko = r.total_score_taiko,
		shiro.users.score_ctb = r.total_score_ctb,
		shiro.users.score_mania = r.total_score_mania,
		shiro.users.ranked_score_std = r.ranked_score_std,
		shiro.users.ranked_score_taiko = r.ranked_score_taiko,
		shiro.users.ranked_score_ctb = r.ranked_score_ctb,
		shiro.users.ranked_score_mania = r.ranked_score_mania,
		shiro.users.accuracy_std = r.avg_accuracy_std,
		shiro.users.accuracy_taiko = r.avg_accuracy_taiko,
		shiro.users.accuracy_ctb = r.avg_accuracy_ctb,
		shiro.users.accuracy_mania = r.avg_accuracy_mania,
		shiro.users.play_count_std = r.playcount_std,
		shiro.users.play_count_taiko = r.playcount_taiko,
		shiro.users.play_count_ctb = r.playcount_ctb,
		shiro.users.play_count_mania = r.playcount_mania,
		shiro.users.user_page = r.userpage_content,
		shiro.users.country = r.country;

-- Relationships
ALTER TABLE shiro.relationships MODIFY blocked TINYINT(1) NOT NULL DEFAULT 0;
INSERT INTO shiro.relationships(origin, target) SELECT user1, user2 FROM ripple.users_relationships;

-- Scores
DROP TABLE shiro.scores;
RENAME TABLE ripple.scores TO shiro.scores;
ALTER TABLE shiro.scores MODIFY beatmap_md5 VARCHAR(35) NOT NULL;
ALTER TABLE shiro.scores ADD COLUMN IF NOT EXISTS (hash VARCHAR(35) NOT NULL);
ALTER TABLE shiro.scores CHANGE COLUMN userid user_id INT(11) NOT NULL;
ALTER TABLE shiro.scores ADD COLUMN IF NOT EXISTS (ranking VARCHAR(2) NOT NULL);
ALTER TABLE shiro.scores MODIFY score BIGINT(20) NOT NULL;
ALTER TABLE shiro.scores CHANGE COLUMN full_combo fc TINYINT(1) NOT NULL;
ALTER TABLE shiro.scores CHANGE COLUMN misses_count miss_count INT(11) NOT NULL;
ALTER TABLE shiro.scores MODIFY time INT(11) NOT NULL DEFAULT 0;
ALTER TABLE shiro.scores CHANGE COLUMN completed passed TINYINT(1) NOT NULL;
ALTER TABLE shiro.scores MODIFY accuracy FLOAT NOT NULL;
ALTER TABLE shiro.scores MODIFY pp FLOAT NOT NULL;
ALTER TABLE shiro.scores ADD COLUMN IF NOT EXISTS (times_watched INT(11) NOT NULL DEFAULT 0);
