CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2489;
	title = 248902;

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
				conv = 248909;
				ctype = 1;
				idx = 1655302;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 248908;
				gtype = 1;
				area = 248901;
				goal = 248904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 248905;
				stype = 2;
				taid = 1;
				title = 248902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 248907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "1655302;";
			}
			CDboTSCheckClrQst
			{
				and = "2473;";
				flink = 1;
				flinknextqid = "2514;";
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
			cont = 248908;
			gtype = 1;
			oklnk = 2;
			area = 248901;
			goal = 248904;
			sort = 248905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 248902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 248914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 248901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072302;";
			}
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
	}
}

