CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 731;
	title = 73102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 4;
			idx = 7313;
			lilnk = 255;
			oklnk = 100;
			prelnk = "3;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "100;2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "3032103;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 73109;
				ctype = 1;
				idx = 3143105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 73108;
				gtype = 1;
				area = 73101;
				goal = 73104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 73105;
				stype = 1;
				taid = 1;
				title = 73102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 73107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 24;
			}
			CDboTSClickNPC
			{
				npcidx = "3143105;";
			}
			CDboTSCheckClrQst
			{
				and = "889;";
				flink = 1;
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
			cont = 73108;
			gtype = 1;
			oklnk = 2;
			area = 73101;
			goal = 73104;
			sort = 73105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 73102;
		}
		CDboTSContReward
		{
			canclnk = 3;
			cid = 100;
			rwdzeny = 0;
			desc = 73114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 73101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;4;";
			usetbl = 1;
		}
	}
}

