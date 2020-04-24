CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1095;
	title = 109502;

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
				conv = 109509;
				ctype = 1;
				idx = 6312103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 109508;
				gtype = 1;
				area = 109501;
				goal = 109504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 109505;
				stype = 2;
				taid = 1;
				title = 109502;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 109507;
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
				npcidx = "6312103;";
			}
			CDboTSCheckClrQst
			{
				and = "918;";
				flink = 1;
				not = 0;
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
			cont = 109508;
			gtype = 1;
			oklnk = 2;
			area = 109501;
			goal = 109504;
			sort = 109505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 109502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 109514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 109501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8032107;";
			}
		}
	}
}

