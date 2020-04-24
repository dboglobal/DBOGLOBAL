CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2641;
	title = 264102;

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
				conv = 264109;
				ctype = 1;
				idx = 4511518;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 264108;
				gtype = 1;
				area = 264101;
				goal = 264104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 264105;
				stype = 2;
				taid = 1;
				title = 264102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 264107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "4511518;";
			}
			CDboTSCheckClrQst
			{
				and = "2640;";
				flink = 1;
				flinknextqid = "2642;";
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
			cont = 264108;
			gtype = 1;
			oklnk = 2;
			area = 264101;
			goal = 264104;
			sort = 264105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 264102;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 264114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 264101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5041150;";
			}
		}
	}
}

