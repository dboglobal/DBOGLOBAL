CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6203;
	title = 620302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSColRgn
			{
				widx = 1;
				cct = 1;
				crt = 0;
				r = "50.000000";
				x = "6344.790039";
				z = "704.780029";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 620320;
			}
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 620307;
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
				npcidx = "3332203;";
			}
			CDboTSCheckClrQst
			{
				and = "6202;";
				flink = 0;
				not = 0;
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
				conv = 620309;
				ctype = 1;
				idx = 3332203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 620308;
				gtype = 1;
				area = 620301;
				goal = 620304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 620305;
				stype = 2;
				taid = 1;
				title = 620302;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 620308;
			gtype = 1;
			oklnk = 2;
			area = 620301;
			goal = 620304;
			sort = 620305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 620302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 620314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 620301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3332203;";
			}
		}
	}
}

