CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2605;
	title = 260502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;4;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "5101102;";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 260520;
			}
		}
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
				r = "15.000000";
				x = "448.000000";
				z = "-2688.000000";
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
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 260507;
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
				npcidx = "5101102;";
			}
			CDboTSCheckClrQst
			{
				and = "2604;";
				flink = 1;
				flinknextqid = "2606;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 260514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 260501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 260509;
				ctype = 1;
				idx = 5101102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 260508;
				area = 260501;
				goal = 260504;
				m0fz = "-2668.000000";
				m0widx = 1;
				scitem = -1;
				sort = 260505;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "448.000000";
				m0ttip = 260515;
				rwd = 100;
				taid = 1;
				title = 260502;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 260508;
			gtype = 2;
			oklnk = 2;
			area = 260501;
			goal = 260504;
			sort = 260505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 260502;
		}
	}
}

