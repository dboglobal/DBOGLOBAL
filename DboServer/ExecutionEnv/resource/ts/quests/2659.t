CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2659;
	title = 265902;

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
				conv = 265909;
				ctype = 1;
				idx = 5553103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 265908;
				gtype = 1;
				area = 265901;
				goal = 265904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 265905;
				stype = 2;
				taid = 1;
				title = 265902;
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 20;
			idx = 26591;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 265907;
			nolnk = 253;
			rm = 0;
			yeslnk = 20;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5553103;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2660;";
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
			cont = 265908;
			gtype = 1;
			oklnk = 2;
			area = 265901;
			goal = 265904;
			sort = 265905;
			prelnk = "20;20;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 265902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 265914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 265901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4652101;";
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
	}
}

