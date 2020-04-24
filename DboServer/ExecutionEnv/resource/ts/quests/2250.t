CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2250;
	title = 225002;

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
				conv = 225009;
				ctype = 1;
				idx = 2611106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 225008;
				gtype = 1;
				area = 225001;
				goal = 225004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 225005;
				stype = 1;
				taid = 1;
				title = 225002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 225007;
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
				npcidx = "2611106;";
			}
			CDboTSCheckClrQst
			{
				and = "2249;";
				flink = 1;
				flinknextqid = "2251;";
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
			cont = 225008;
			gtype = 1;
			oklnk = 2;
			area = 225001;
			goal = 225004;
			sort = 225005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 225002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 225014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 225001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7091114;";
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

