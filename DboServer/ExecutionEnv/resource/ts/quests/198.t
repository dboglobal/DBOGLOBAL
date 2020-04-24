CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 198;
	title = 19802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 19820;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 19808;
			gtype = 1;
			oklnk = 2;
			area = 19801;
			goal = 19804;
			sort = 19805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 19802;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSTeleport
			{
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 19814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 19801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8512101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 19809;
				ctype = 1;
				idx = 8512101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 19808;
				gtype = 1;
				area = 19801;
				goal = 19804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 19805;
				stype = 64;
				taid = 1;
				title = 19802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 19807;
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
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "8512101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

