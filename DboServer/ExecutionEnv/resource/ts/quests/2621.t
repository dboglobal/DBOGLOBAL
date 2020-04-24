CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2621;
	title = 262102;

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
				conv = 262109;
				ctype = 1;
				idx = 3182108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 262108;
				gtype = 1;
				area = 262101;
				goal = 262104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 262105;
				stype = 1;
				taid = 1;
				title = 262102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 262114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 262101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5313102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 262107;
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
				npcidx = "3182108;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2622;";
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
			cont = 262108;
			gtype = 1;
			oklnk = 2;
			area = 262101;
			goal = 262104;
			sort = 262105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 262102;
		}
	}
}

