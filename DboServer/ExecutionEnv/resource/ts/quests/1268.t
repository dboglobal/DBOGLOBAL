CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1268;
	title = 126802;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 126809;
				ctype = 1;
				idx = 4261104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 126808;
				gtype = 3;
				area = 126801;
				goal = 126804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 126805;
				stype = 1;
				taid = 1;
				title = 126802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 126807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSClickNPC
			{
				npcidx = "4261104;";
			}
			CDboTSCheckClrQst
			{
				and = "1267;";
				flink = 1;
				flinknextqid = "1481;";
				not = 0;
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
			cont = 126808;
			gtype = 3;
			oklnk = 2;
			area = 126801;
			goal = 126804;
			sort = 126805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 126802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 126814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 126801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "842;";
				widx = 1;
			}
		}
	}
}

