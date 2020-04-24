CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1226;
	title = 122602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 122614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 122601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4111101;";
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
				conv = 122609;
				ctype = 1;
				idx = 5062102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 122608;
				gtype = 3;
				area = 122601;
				goal = 122604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 122605;
				stype = 1;
				taid = 1;
				title = 122602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 122607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSClickNPC
			{
				npcidx = "5062102;";
			}
			CDboTSCheckClrQst
			{
				and = "1225;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 122608;
			gtype = 3;
			oklnk = 2;
			area = 122601;
			goal = 122604;
			sort = 122605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 122602;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
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
	}
}

