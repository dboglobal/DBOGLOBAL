CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6221;
	title = 622102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 622108;
			gtype = 1;
			oklnk = 2;
			area = 622101;
			goal = 622104;
			sort = 622105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 622102;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 622109;
				ctype = 1;
				idx = 4072113;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 622108;
				gtype = 1;
				area = 622101;
				goal = 622104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 622105;
				stype = 2;
				taid = 1;
				title = 622102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 622114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 622101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 622107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 38;
			}
			CDboTSClickNPC
			{
				npcidx = "4072113;";
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

