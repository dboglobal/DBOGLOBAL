CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 221;
	title = 22102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 5;
			idx = 2213;
			lilnk = 255;
			oklnk = 100;
			prelnk = "4;";
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 5;

			CDboTSClickNPC
			{
				npcidx = "2413110;";
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 2211;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 22108;
				gtype = 1;
				area = 22101;
				goal = 22104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 22105;
				stype = 1;
				taid = 1;
				title = 22102;
			}
			CDboTSActNPCConv
			{
				conv = 22109;
				ctype = 1;
				idx = 4211101;
				taid = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 4;
			cid = 100;
			rwdzeny = 0;
			desc = 22114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 22101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;5;";
			usetbl = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 22108;
			gtype = 1;
			oklnk = 2;
			area = 22101;
			goal = 22104;
			sort = 22105;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 22102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 22107;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
		}
	}
}

