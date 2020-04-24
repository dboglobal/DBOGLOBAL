CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1409;
	title = 140902;

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
				conv = 140909;
				ctype = 1;
				idx = 1352101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 140908;
				gtype = 2;
				area = 140901;
				goal = 140904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 140905;
				stype = 2;
				taid = 1;
				title = 140902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 140907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "1352101;";
			}
			CDboTSCheckClrQst
			{
				and = "1408;";
				flink = 1;
				flinknextqid = "1450;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 140908;
			gtype = 2;
			oklnk = 2;
			area = 140901;
			goal = 140904;
			sort = 140905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 140902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 140914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 140901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2803101;";
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
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

