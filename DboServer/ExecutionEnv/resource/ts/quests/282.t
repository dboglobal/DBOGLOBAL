CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 282;
	title = 28202;

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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 28207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 9;
			}
			CDboTSClickNPC
			{
				npcidx = "4751303;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 28214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 28201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "55;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 28209;
				ctype = 1;
				idx = 4751303;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 28208;
				gtype = 1;
				area = 28201;
				goal = 28204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 28205;
				stype = 1;
				taid = 1;
				title = 28202;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 28208;
			gtype = 1;
			oklnk = 2;
			area = 28201;
			goal = 28204;
			sort = 28205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 28202;
		}
	}
}

