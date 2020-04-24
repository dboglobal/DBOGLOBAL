CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11634;
	title = 1100943;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 254;
			prelnk = "15;";

			CDboTSActSendSvrEvt
			{
				id = 16290;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "16;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 16;
			prelnk = "6;";

			CDboTSActSendSvrEvt
			{
				id = 16240;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 15;

			CDboTSSkillUse
			{
				idx = -1;
				rp = 0;
				stype = 2;
				tidx = -1;
				ttype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 6;
			prelnk = "1;1;";

			CDboTSActRegQInfo
			{
				cont = 1100941;
				gtype = -1;
				area = 1100407;
				goal = 1100944;
				grade = -1;
				rwd = 255;
				sort = 1100406;
				stype = 32;
				taid = 1;
				title = 1100943;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100940;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 6201101;
			}
			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 11633;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100952;
			gt = 0;
			oi = 6201101;
		}
	}
}

