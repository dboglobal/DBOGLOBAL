CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 509;
	title = 50902;

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
				conv = 50909;
				ctype = 1;
				idx = 3171101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 50908;
				gtype = 1;
				area = 50901;
				goal = 50904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 50905;
				stype = 2;
				taid = 1;
				title = 50902;
			}
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 50907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "3171101;";
			}
			CDboTSCheckClrQst
			{
				and = "504;";
				flink = 1;
				flinknextqid = "515;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 50908;
			gtype = 1;
			oklnk = 2;
			area = 50901;
			goal = 50904;
			sort = 50905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 50902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 50914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 50901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4141108;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

