CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6201;
	title = 620102;

	CNtlTSGroup
	{
		gid = 0;

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
			cont = 620108;
			gtype = 1;
			oklnk = 2;
			area = 620101;
			goal = 620104;
			sort = 620105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 620102;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 620109;
				ctype = 1;
				idx = 4751104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 620108;
				gtype = 1;
				area = 620101;
				goal = 620104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 620105;
				stype = 2;
				taid = 1;
				title = 620102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 620114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 620101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 620107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "4751104;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

