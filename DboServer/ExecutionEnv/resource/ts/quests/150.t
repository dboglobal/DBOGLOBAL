CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 150;
	title = 15002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 15007;
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
				minlvl = 3;
			}
			CDboTSClickObject
			{
				objidx = "29;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActObjConv
			{
				conv = 15009;
				idx = 29;
				widx = 1;
				ctype = 1;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 15008;
				gtype = 1;
				area = 15001;
				goal = 15004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 15005;
				stype = 1;
				taid = 1;
				title = 15002;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 15008;
			gtype = 1;
			oklnk = 2;
			area = 15001;
			goal = 15004;
			sort = 15005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 15002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 15014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 15001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5312101;";
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
	}
}

