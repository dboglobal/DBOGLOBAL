CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2249;
	title = 224902;

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
				conv = 224909;
				ctype = 1;
				idx = 7091114;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 224908;
				gtype = 1;
				area = 224901;
				goal = 224904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 224905;
				stype = 1;
				taid = 1;
				title = 224902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 224907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "7091114;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2250;";
				not = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 224908;
			gtype = 1;
			oklnk = 2;
			area = 224901;
			goal = 224904;
			sort = 224905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 224902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 224914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 224901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611106;";
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

