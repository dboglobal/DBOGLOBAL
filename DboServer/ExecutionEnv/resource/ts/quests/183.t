CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 183;
	title = 18302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 18309;
				ctype = 1;
				idx = 8031501;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 18308;
				m1fx = "4236.000000";
				area = 18301;
				goal = 18304;
				m1fy = "0.000000";
				m1ttip = 18316;
				scitem = -1;
				sort = 18305;
				stype = 2;
				m1fz = "4733.000000";
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 18302;
				gtype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 18307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSClickNPC
			{
				npcidx = "8031501;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 18308;
			gtype = 1;
			oklnk = 2;
			area = 18301;
			goal = 18304;
			sort = 18305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 18302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 18314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 18301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2801106;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

