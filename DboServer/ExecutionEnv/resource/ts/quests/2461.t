CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2461;
	title = 246102;

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
				conv = 246109;
				ctype = 1;
				idx = 3173112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 246108;
				gtype = 1;
				area = 246101;
				goal = 246104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 246105;
				stype = 2;
				taid = 1;
				title = 246102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 246114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 246101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031502;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 246107;
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
				npcidx = "3173112;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2462;";
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
			cont = 246108;
			gtype = 1;
			oklnk = 2;
			area = 246101;
			goal = 246104;
			sort = 246105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 246102;
		}
	}
}

