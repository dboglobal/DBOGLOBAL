CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2545;
	title = 254502;

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
				conv = 254509;
				ctype = 1;
				idx = 5101101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 254508;
				gtype = 1;
				area = 254501;
				goal = 254504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 254505;
				stype = 2;
				taid = 1;
				title = 254502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 254508;
			gtype = 1;
			oklnk = 2;
			area = 254501;
			goal = 254504;
			sort = 254505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 254502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 254514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 254501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1121101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 254507;
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
				npcidx = "5101101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2546;";
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
	}
}

